#include "EnemyBase.h"

class LongDisEnemy : public EnemyBase
{
public:

	LongDisEnemy(Bike* bike);

	//パラメータ設定		//override上書き
	void SetParam(void) override;
	void Update(void) override;

	void UpdatePlay(void) override;
	void ProcessMove(void) override;



};

