#include "pixelpch.h"
#include "Logger.h"
#include <cstdarg>
#include <ctime>

namespace Pixel {
    static LogFormat* formatter = nullptr;

    void TimestampLogCommand::RunCommand(va_list& args, const char* input) {
        const std::time_t now = std::time(nullptr); 
        const std::tm calendar_time = *std::localtime(std::addressof(now));

        command_output = std::to_string(calendar_time.tm_hour) + ":" + std::to_string(calendar_time.tm_min) + ":" + std::to_string(calendar_time.tm_sec);
    }

    void TimestampLogCommand::ProcessArgs(va_list& args) { }

    void LogCommand::AddToOutput(std::string& output, std::vector<LogCommand*>& commands) {
        size_t pos = GetPosition();
        size_t size = GetCommandOutput().size();
        output.insert(GetPosition(), GetCommandOutput());
        for (auto& command : commands)
            if (pos < command->GetPosition())
                command->SetPosition(command->GetPosition() + size);
    }

    void UserDefinedStringCommand::RunCommand(va_list& args, const char* input) { }

    void UserDefinedStringCommand::ProcessArgs(va_list& args) {
          command_output = va_arg(args, char*);
    }

    void UserLogCommand::RunCommand(va_list& args, const char* input) {
        std::string user_in;
        bool found_new_arg = false;
        while (*input != '\0') {
            if (*input == '%') {
                found_new_arg = true;
            }
            else if (found_new_arg) {
                switch (*input) {
                case 'd': user_in += std::to_string(va_arg(args, int)); break;
                case 'f': user_in += std::to_string(va_arg(args, float)); break;
                case 's': user_in += va_arg(args, char*); break;
                }
                found_new_arg = false;
            }
            else
                user_in += *input;


            ++input;
        }
        SetCommandOutput(user_in);
    }

    void UserLogCommand::ProcessArgs(va_list& args) { }

    void NewLineLogCommand::RunCommand(va_list& args, const char* input) {
        command_output = '\n';
    }

    void NewLineLogCommand::ProcessArgs(va_list& args) { }

    static std::map<std::string, CreateLogCommandFn> COMMANDS;

    void InitCommands();

    void InitializeLoggingSystem() {
        InitCommands();
    }

    void InitCommands() {
        COMMANDS["{ts}"] = TimestampLogCommand::Create;
        COMMANDS["{s}"] = UserDefinedStringCommand::Create;
        COMMANDS["{l}"] = UserLogCommand::Create;
        COMMANDS["\n"] = NewLineLogCommand::Create;
    }

    void FindAllOccurances(std::vector<size_t>& vec, std::string data, std::string to_search) {
        size_t pos = data.find(to_search);
        while (pos != std::string::npos)
        {
            vec.push_back(pos);
            pos = data.find(to_search, pos + to_search.size());
        }
    }

    void LogFormat::Init(const char* format, ...) {
        va_list args;
        va_start(args, format);
 
        std::string input = format;

        for (auto& current_command : COMMANDS) {
            std::vector<size_t> positions;
            FindAllOccurances(positions, input, current_command.first);
            for (auto& position : positions) {
                commands.push_back(current_command.second());
                commands.back()->SetCommand(current_command.first);
                commands.back()->SetPosition(position);
                commands.back()->ProcessArgs(args);
            }
        }

        for (auto& command : commands) {
            input.erase(command->GetPosition(), command->GetCommand().size());
            for (auto& pos_test : commands)
                if (command->GetPosition() < pos_test->GetPosition())
                    pos_test->SetPosition(pos_test->GetPosition() - command->GetCommand().size());
        }

        for (auto& command : commands)
            command->GetResetingPosition();

        output = input;

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

            for (auto& command : formatter->GetCommands())
                command->ResetPosition();

            for (auto& command : formatter->GetCommands()) {
                command->RunCommand(args, fmt);
                command->AddToOutput(output, formatter->GetCommands());
            }

            printf("%s", output.c_str());
            va_end(args);
        }
    }
}