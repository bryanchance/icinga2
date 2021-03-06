/* Icinga 2 | (c) 2012 Icinga GmbH | GPLv2+ */

#ifndef TROUBLESHOOTCOMMAND_H
#define TROUBLESHOOTCOMMAND_H

#include "cli/clicommand.hpp"
#include "base/i2-base.hpp"
#include "base/dictionary.hpp"

namespace icinga
{

/**
 * The "troubleshoot" command.
 *
 * @ingroup cli
 */
class TroubleshootCommand final : public CLICommand
{
public:
	DECLARE_PTR_TYPEDEFS(TroubleshootCommand);

	String GetDescription() const override;
	String GetShortDescription() const override;
	int Run(const boost::program_options::variables_map& vm, const std::vector<std::string>& ap) const override;
	void InitParameters(boost::program_options::options_description& visibleDesc,
		boost::program_options::options_description& hiddenDesc) const override;

private:
	class InfoLog;
	class InfoLogLine;
	static bool GeneralInfo(InfoLog& log, const boost::program_options::variables_map& vm);
	static bool FeatureInfo(InfoLog& log, const boost::program_options::variables_map& vm);
	static bool ObjectInfo(InfoLog& log, const boost::program_options::variables_map& vm,
		Dictionary::Ptr& logs, const String& path);
	static bool ReportInfo(InfoLog& log, const boost::program_options::variables_map& vm,
		Dictionary::Ptr& logs);
	static bool ConfigInfo(InfoLog& log, const boost::program_options::variables_map& vm);

	static int Tail(const String& file, const int numLines, InfoLog& log);
	static bool CheckFeatures(InfoLog& log);
	static void GetLatestReport(const String& filename, time_t& bestTimestamp, String& bestFilename);
	static bool PrintCrashReports(InfoLog& log);
	static bool PrintFile(InfoLog& log, const String& path);
	static bool CheckConfig();
	static void CheckObjectFile(const String& objectfile, InfoLog& log, InfoLog *OFile, const bool objectConsole,
		Dictionary::Ptr& logs, std::set<String>& configs);
	static bool PrintVarsFile(const String& path, const bool console);
	static void PrintLoggers(InfoLog& log, Dictionary::Ptr& logs);
	static void PrintObjectOrigin(InfoLog& log, const std::set<String>& configSet);
};

}
#endif /* TROUBLESHOOTCOMMAND_H */
