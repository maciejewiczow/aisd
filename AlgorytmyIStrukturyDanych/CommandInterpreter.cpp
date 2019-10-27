#include "CommandInterpreter.h"
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>

const std::string CommandInterpreter::help_cmd = "help";
const std::string CommandInterpreter::exit_cmd = "exit";

void CommandInterpreter::printHelp(const std::string command = "") const {
    if (command.size() == 0) {
        for (auto& handler : getHandlers()) {
            std::cout << handler.first << ":\t" << handler.second.desc_short << std::endl;
        }
        std::cout << exit_cmd << ":\tends program execution" << std::endl;
    }
    else {
        try {
            auto handler = getHandlerByCommandname(command);
            std::cout << command << ":\t" << handler.desc_long << std::endl;
        }
        catch (const std::exception&) {
            noHandlerError(command);
        }
    }
}

void CommandInterpreter::noHandlerError(const std::string command_name) const {
    if (command_name != exit_cmd) {
        std::cout << "There is no command " << command_name << "." << std::endl
                  << "To see list of all availible commands, type 'help'." << std::endl
                  << "To get help about specific command, type 'help <command name>'" << std::endl;
    }
}

void CommandInterpreter::init() {
    callback helpHandler = [&](argv_iterator& begin, argv_iterator& end) {
        if (begin != end) {
            printHelp(*begin);
        }
        else {
            printHelp();
        }
    };

    on(help_cmd,
        helpHandler,
        "lists all commands with their descriptions",
        "usage: 'help' or 'help <command name>'.\nWith no arguments, lists all commands with their "
        "short descriptions.\nWhen command name is passed as an argument, displays long "
        "description of specified command.");
}

const CommandInterpreter::Handler&
    CommandInterpreter::getHandlerByCommandname(const std::string command) const {
    auto handler = handlers.find(command);

    if (handler == handlers.end()) throw std::invalid_argument("Unknown command");

    return handler->second;
}

CommandInterpreter::CommandInterpreter() {
    init();
}

CommandInterpreter::CommandInterpreter(const handlerMap& cmds) : handlers(cmds) {
    init();
}

CommandInterpreter::CommandInterpreter(
    std::initializer_list<std::pair<const std::string, Handler>> args)
    : handlers(args) {
    init();
}

CommandInterpreter::CommandInterpreter(const std::string p) : prompt(p) {}

void CommandInterpreter::on(const std::string& cmd,
    callback fn,
    const std::string& desc_short,
    const std::string& desc_long) {

    handlers.emplace(cmd, Handler(fn, desc_short, desc_long));
}

void CommandInterpreter::on(const std::string& cmd,
    CommandInterpreter& handler,
    const std::string& desc_short,
    const std::string& desc_long) {
    on(cmd,
        [&handler, desc_long, desc_short](argv_iterator& begin, argv_iterator& end) {
            if (begin == end) {
                std::cout << "This command needs more arguments." << std::endl;
                if (desc_long != "") std::cout << desc_long << std::endl;
                return;
            }

            handler.execute(*(begin++), begin, end);
        },
        desc_short,
        desc_long);
}

const CommandInterpreter::handlerMap& CommandInterpreter::getHandlers() const {
    return handlers;
}

void CommandInterpreter::execute(const std::string command,
    argv_iterator argv_begin,
    argv_iterator argv_end) const {

    try {
        auto handler = getHandlerByCommandname(command);

        try {
            handler.fn(argv_begin, argv_end);
        }
        catch (CommandInterpreter::HandlerException& e) {
            std::cerr << e.what() << std::endl;
            std::cout << handler.desc_long << std::endl;
        }
    }
    catch (std::invalid_argument&) {
        noHandlerError(command);
    }
}

bool CommandInterpreter::promptOnce() {
    if (onPromptCallback) onPromptCallback();

    std::string command;

    std::cout << prompt;
    getline(std::cin, command);

    if (command.size() == 0) return true; // skip to next command input

    std::stringstream cmd_stream(command);
    std::remove_reference<argv_iterator>::type begin(cmd_stream);
    std::remove_reference<argv_iterator>::type end;

    const std::string command_name = *(begin++);

    execute(command_name, begin, end);

    return command_name != exit_cmd;
}

void CommandInterpreter::promptUntilExit() {
    while (promptOnce()) {
    }
}

void CommandInterpreter::onPrompt(std::function<void()> cb) {
    onPromptCallback = cb;
}

CommandInterpreter::~CommandInterpreter() {}

CommandInterpreter::Handler::Handler(callback fn,
    const std::string d_short,
    const std::string d_long)
    : fn(fn), desc_short(d_short), desc_long(d_long) {}
