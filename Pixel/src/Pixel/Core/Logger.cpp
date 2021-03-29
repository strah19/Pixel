#include "pixelpch.h"
#include "Logger.h"
#include <cstdarg>
#include <ctime>

namespace Pixel {
    static LogFormat* formatter = nullptr;

    void TimestampLogCommand::RunCommand() {
        const std::time_t now = std::time(nullptr); 
        const std::tm calendar_time = *std::localtime(std::addressof(now));

        command_output = std::to_string(calendar_time.tm_hour) + ":" + std::to_string(calendar_time.tm_min) + ":" + std::to_string(calendar_time.tm_sec);
    }

    void TimestampLogCommand::ProcessArgs(va_list& args) { }

    void UserDefinedStringCommand::RunCommand() { }

    void UserDefinedStringCommand::ProcessArgs(va_list& args) {
          command_output = va_arg(args, char*);
    }

    void UserLogCommand::RunCommand() { }

    void UserLogCommand::ProcessArgs(va_list& args) { }

    void LogFormat::Init(const char* format, ...) {
        va_list args;
        va_start(args, format);

        bool found_new_arg = false;

        std::string command;
        std::map<std::string, CreateLogCommandFn> creators;

        creators["%{ts}"] = TimestampLogCommand::Create;
        creators["%s"] = UserDefinedStringCommand::Create;
        creators["%l"] = UserLogCommand::Create;

        size_t count = 0;
        while(*format != '\0') {
            if (*format == LogCommand::COMMAND_ID) {
                found_new_arg = true;
                command += *format;
            }
            else if (found_new_arg) {
                command += *format;
                for (auto& c : creators) {
                    if (c.first == command) {
                        commands.push_back(c.second());
                        count -= commands.back()->GetCommand().size();
                        commands.back()->SetPosition(count);
                        commands.back()->ProcessArgs(args);

                        command = "";
                        found_new_arg = false;
                        break;
                    }
                }
            }
            else
                output += *format;
            ++format;
            count++;
        }
        /*
        //Put in log command class...As function called PushToOutput
        for (auto& com : commands) {
            if (com->GetCommand() == "%s") {
                output.insert(com->GetPosition() + 1, com->GetCommandOutput());
            }
        }
        */
  
        va_end(args);
	}

    void SetLogFormat(LogFormat* log_format) {
        formatter = log_format;
    }

    void Log(const char* fmt, ...) {
        if (formatter) {
            va_list args;
            va_start(args, fmt);
            std::string output = formatter->GetLeftOutput();

            for (auto& command : formatter->GetCommands()) {
                if (command->GetCommand() == "%l") {
                    std::string user_in;
                    bool found_new_arg = false;
                    while (*fmt != '\0') {
                        if (*fmt == LogCommand::COMMAND_ID) {
                            found_new_arg = true;
                        }
                        else if (found_new_arg) {
                            switch (*fmt) {
                            case 'd': user_in += std::to_string(va_arg(args, int)); break;
                            case 'f': user_in += std::to_string(va_arg(args, float)); break;
                            case 's': user_in += va_arg(args, char*); break;
                            }
                            found_new_arg = false;
                        }
                        else
                            user_in += *fmt;


                        ++fmt;
                    }
                    command->SetCommandOutput(user_in);
                }
                command->RunCommand();
                size_t pos = command->GetPosition() + 1;
                size_t size = command->GetCommandOutput().size();
                output.insert(command->GetPosition() + 1, command->GetCommandOutput());
                for (auto& pos_test : formatter->GetCommands()) 
                    if (pos < pos_test->GetPosition())
                        pos_test->SetPosition(pos_test->GetPosition() + size);
            }

            printf("%s", output.c_str());

            va_end(args);
        }
    }
}