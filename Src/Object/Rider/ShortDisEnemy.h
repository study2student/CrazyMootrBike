#include "EnemyBase.h"

class ShortDisEnemy : public EnemyBase
{
public:

	ShortDisEnemy();

	//ƒpƒ‰ƒ[ƒ^İ’è		//overrideã‘‚«
	void SetParam(void) override;
	void Update(void) override;

	void UpdatePlay(void) override;
	void ProcessMove(void) override;


};

