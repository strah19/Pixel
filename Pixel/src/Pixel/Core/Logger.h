#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <map>

namespace Pixel {
	class LogCommand {
	public:
		LogCommand() : command_name("") { }
		virtual void RunCommand(va_list& args, const char* input) = 0;
		virtual void ProcessArgs(va_list& args) = 0;
		virtual void AddToOutput(std::string& output, std::vector<LogCommand*>& commands);

		inline const std::string GetCommand() const { return command_name; }
		inline const std::string GetCommandOutput() const { return command_output; }
		inline size_t GetPosition() const { return starting_position; }

		inline void SetCommand(const std::string& command_name) { this->command_name = command_name; };
		inline void SetCommandOutput(const std::string& command_output) { this->command_output = command_output; };
		inline void SetPosition(size_t position) { starting_position = position; }
		inline void ResetPosition() { starting_position = reseting_position; }
		inline void GetResetingPosition() { reseting_position = starting_position; }
	protected:
		std::string command_name;
		std::string command_output;
		size_t starting_position = 0;
		size_t reseting_position = 0;
	};

	typedef LogCommand* (__stdcall* CreateLogCommandFn)(void);

	class TimestampLogCommand : public LogCommand {
	public:
		TimestampLogCommand() { }
		void RunCommand(va_list& args, const char* input) override;
		void ProcessArgs(va_list& args) override;
		static LogCommand* __stdcall Create() { return new TimestampLogCommand(); }
	};

	class UserDefinedStringCommand : public LogCommand {
	public:
		UserDefinedStringCommand() { }
		void RunCommand(va_list& args, const char* input) override;
		void ProcessArgs(va_list& args) override;
		static LogCommand* __stdcall Create() { return new UserDefinedStringCommand(); }
	};

	class UserLogCommand : public LogCommand {
	public:
		UserLogCommand() { }
		void RunCommand(va_list& args, const char* input) override;
		void ProcessArgs(va_list& args) override;
		static LogCommand* __stdcall Create() { return new UserLogCommand(); }
	};

	class NewLineLogCommand : public LogCommand {
	public:
		NewLineLogCommand() { }
		void RunCommand(va_list& args, const char* input) override;
		void ProcessArgs(va_list& args) override;
		static LogCommand* __stdcall Create() { return new NewLineLogCommand(); }
	};

	class LogFormat {
	public:
		void Init(const char* format, ...);
		std::vector<LogCommand*>& GetCommands() { return commands; }
		std::string GetLeftOutput() { return output; }
	private:
		std::vector<LogCommand*> commands;
		std::string output;
	};

	void InitializeLoggingSystem();
	void SetLogFormat(LogFormat* log_format);
	void Log(const char* fmt, ...);
}

#endif // !LOGGER_H
