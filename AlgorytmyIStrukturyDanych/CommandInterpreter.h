#pragma once
#include <functional>
#include <iterator>
#include <map>
#include <vector>

class CommandInterpreter {
public:
    static const std::string help_cmd;
    static const std::string exit_cmd;

    // does not work with const string - no idea why
    // TODO: find out why
    using argv_iterator = std::istream_iterator<std::string>&;
    using callback = std::function<void(argv_iterator argv_begin, argv_iterator argv_end)>;

    struct Handler {
        const std::string desc_short;
        const std::string desc_long;
        callback fn;

        Handler(callback, const std::string, const std::string);
    };

private:
    using handlerMap = std::map<const std::string, Handler>;

    std::string prompt = "> ";
    handlerMap handlers;
    std::function<void()> onPromptCallback;
    std::function<void(const std::string&)> commandSuccessCb;

    void printHelp(const std::string) const;
    void noHandlerError(const std::string) const;
    void init();
    const Handler& getHandlerByCommandname(const std::string) const;

public:
    CommandInterpreter();
    CommandInterpreter(const handlerMap& cmds);
    CommandInterpreter(std::initializer_list<std::pair<const std::string, Handler>> args);
    CommandInterpreter(const std::string p);

    class HandlerException : public std::exception {
    public:
        HandlerException() noexcept : exception(){};
        explicit HandlerException(const char* mess) noexcept : exception(mess){};
    };

    void on(const std::string& cmd,
        callback fn,
        const std::string& desc_short = "",
        const std::string& desc_long = "");

    void on(const std::string& cmd,
        CommandInterpreter& handler,
        const std::string& desc_short = "",
        const std::string& desc_long = "");

    const handlerMap& getHandlers() const;

    void execute(const std::string command, argv_iterator argv_begin, argv_iterator argv_end) const;

    bool promptOnce();
    void promptUntilExit();
    void onPrompt(std::function<void()> cb);
    void onCommandSuccess(std::function<void(const std::string&)>);

    ~CommandInterpreter();
};
