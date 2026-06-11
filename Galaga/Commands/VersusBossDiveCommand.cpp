#include "VersusBossDiveCommand.h"

#include "VersusBossComponent.h"

VersusBossDiveCommand::VersusBossDiveCommand(VersusBossComponent& boss)
	: m_Boss(boss)
{

}

void VersusBossDiveCommand::Execute(float)
{
	m_Boss.StartDive();
}