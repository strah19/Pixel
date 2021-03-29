#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <map>

namespace Pixel {
	class LogCommand {
	public:
		LogCommand(const std::string& command_name) : command_name(COMMAND_ID + command_name) { }
		virtual void RunCommand() = 0;
		virtual void ProcessArgs(va_list& args) = 0;

		inline const std::string GetCommand() const { return command_name; }
		inline const std::string GetCommandOutput() const { return command_output; }
		inline size_t GetPosition() const { return starting_position; }

		inline void SetCommand(const std::string& command_name) { this->command_name = command_name; };
		inline void SetCommandOutput(const std::string& command_output) { this->command_output = command_output; };
		inline void SetPosition(size_t position) { starting_position = position; }

		static const char COMMAND_ID = '%';
	protected:
		std::string command_name;
		std::string command_output;
		size_t starting_position = 0;
	};

	typedef std::unique_ptr<LogCommand> (__stdcall* CreateLogCommandFn)(void);

	class TimestampLogCommand : public LogCommand {
	public:
		TimestampLogCommand() : LogCommand("{ts}") { }
		void RunCommand();
		void ProcessArgs(va_list& args);
		static std::unique_ptr<LogCommand> __stdcall Create() { return std::make_unique<TimestampLogCommand>(); }
	};

	class UserDefinedStringCommand : public LogCommand {
	public:
		UserDefinedStringCommand() : LogCommand("s") { }
		void RunCommand();
		void ProcessArgs(va_list& args);
		static std::unique_ptr<LogCommand> __stdcall Create() { return std::make_unique<UserDefinedStringCommand>(); }
	};

	class UserLogCommand : public LogCommand {
	public:
		UserLogCommand() : LogCommand("l") { }
		void RunCommand();
		void ProcessArgs(va_list& args);
		static std::unique_ptr<LogCommand> __stdcall Create() { return std::make_unique<UserLogCommand>(); }
	};

	class LogCommandFactory {
	public:

	};

	class LogFormat {
	public:
		void Init(const char* format, ...);
		std::vector<std::unique_ptr<LogCommand>>& GetCommands() { return commands; }
		std::string GetLeftOutput() { return output; }
	private:
		std::vector<std::unique_ptr<LogCommand>> commands;
		std::string output;
	};

	void SetLogFormat(LogFormat* log_format);
	void Log(const char* fmt, ...);
}

#endif // !LOGGER_H
