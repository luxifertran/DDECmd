////////////////////////////////////////////////////////////////////////////////
//! \file   PokeCmd.cpp
//! \brief  The PokeCmd class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "PokeCmd.hpp"
#include "CmdLineArgs.hpp"
#include <Core/CmdLineException.hpp>
#include <Core/AnsiWide.hpp>
#include <NCL/DDEClient.hpp>
#include <NCL/DDECltConvPtr.hpp>
#include <NCL/DDEData.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The table of command specific command line switches.

static Core::CmdLineSwitch s_switches[] = 
{
	{ USAGE,	TXT("?"),	NULL,			Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,		NULL,			TXT("Display the command syntax")	},
	{ USAGE,	NULL,		TXT("help"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,		NULL,			TXT("Display the command syntax")	},
	{ SERVER,	TXT("s"),	TXT("server"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("server"),	TXT("The DDE Server name")			},
	{ TOPIC,	TXT("t"),	TXT("topic"), 	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("topic"),	TXT("The DDE Server topic")			},
	{ ITEM,		TXT("i"),	TXT("item"), 	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("item"),	TXT("The item name(s)")				},
	{ VALUE,	TXT("v"),	TXT("value"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("value"),	TXT("The value to poke")			},
};
static size_t s_switchCount = ARRAY_SIZE(s_switches);

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

PokeCmd::PokeCmd(int argc, tchar* argv[])
	: Command(s_switches, s_switches+s_switchCount, argc, argv)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

PokeCmd::~PokeCmd()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Get the description of the command.

const tchar* PokeCmd::getDescription()
{
	return TXT("Set the value for a single item");
}

////////////////////////////////////////////////////////////////////////////////
//! Get the expected command usage.

const tchar* PokeCmd::getUsage()
{
	return TXT("USAGE: DDECmd poke --server <server> --topic <topic> --item <item> --value <value>");
}

////////////////////////////////////////////////////////////////////////////////
//! The implementation of the command.

int PokeCmd::doExecute(tostream& /*out*/, tostream& /*err*/)
{
	// Validate the command line arguments.
	if (!m_parser.isSwitchSet(SERVER))
		throw Core::CmdLineException(TXT("No DDE server name specified [--server]"));

	if (!m_parser.isSwitchSet(TOPIC))
		throw Core::CmdLineException(TXT("No DDE server topic specified [--topic]"));

	if (!m_parser.isSwitchSet(ITEM))
		throw Core::CmdLineException(TXT("No item specified [--item]"));

	if (!m_parser.isSwitchSet(VALUE))
		throw Core::CmdLineException(TXT("No value specified [--value]"));

	// Extract command line argument values.
	tstring server = m_parser.getSwitchValue(SERVER);
	tstring topic  = m_parser.getSwitchValue(TOPIC);
	tstring item   = m_parser.getSwitchValue(ITEM);
	tstring value  = m_parser.getSwitchValue(VALUE);

	// Open the conversation.
	CDDEClient client;
	DDE::CltConvPtr conv(client.CreateConversation(server.c_str(), topic.c_str()));

	// Set the value.
	std::string ansiValue(T2A(value.c_str()));

	conv->Poke(item.c_str(), CF_TEXT, ansiValue.data(), ansiValue.length());

	return EXIT_SUCCESS;
}
