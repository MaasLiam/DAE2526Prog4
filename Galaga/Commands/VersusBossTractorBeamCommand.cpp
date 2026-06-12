#include "VersusBossTractorBeamCommand.h"

#include "VersusBossComponent.h"

namespace galaga
{
	VersusBossTractorBeamCommand::VersusBossTractorBeamCommand(VersusBossComponent& boss)
		: m_Boss(boss)
	{
	
	}

	void VersusBossTractorBeamCommand::Execute(float)
	{
		m_Boss.StartTractorBeam();
	}
}