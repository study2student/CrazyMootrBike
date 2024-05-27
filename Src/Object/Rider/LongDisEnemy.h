#include "EnemyBase.h"

class LongDisEnemy : public EnemyBase
{
public:

	LongDisEnemy(Bike* bike, VECTOR loopStagePos, VECTOR localPos);

	//ƒpƒ‰ƒ[ƒ^İ’è		//overrideã‘‚«
	void SetParam(void) override;
	void Update(void) override;

	void UpdatePlay(void) override;
	void ProcessMove(void) override;



};

