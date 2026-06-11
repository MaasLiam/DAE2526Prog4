#include "VersusBossTractorBeamCommand.h"

#include "VersusBossComponent.h"

VersusBossTractorBeamCommand::VersusBossTractorBeamCommand(VersusBossComponent& boss)
	: m_Boss(boss)
{

}

void VersusBossTractorBeamCommand::Execute(float)
{
	m_Boss.StartTractorBeam();
}