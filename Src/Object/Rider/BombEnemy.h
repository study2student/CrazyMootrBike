#include "EnemyBase.h"

class BombEnemy : public EnemyBase
{
public:

	BombEnemy();

	//ƒpƒ‰ƒ[ƒ^İ’è		//overrideã‘‚«
	void SetParam(void) override;
	void Update(void) override;

	void UpdatePlay(void) override;
	void ProcessMove(void) override;


};

