#pragma once
#include "Game.h"
#include <vector>

class ThrashTheCache final : public Game
{
public:
    void Initialize() override;
    void Render() override;

private:
    void RunExercise1();
	void RunExercise2();
    void RunExercise2Alt();

    int m_Samples{ 10 };
    std::vector<float> m_Ex1Timings;
	std::vector<float> m_Ex2Timings;
    std::vector<float> m_Ex2TimingsAlt;
};