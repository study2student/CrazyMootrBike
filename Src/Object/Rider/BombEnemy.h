#include "EnemyBase.h"

class BombEnemy : public EnemyBase
{
public:

	BombEnemy();

	//パラメータ設定		//override上書き
	void SetParam(void) override;
	void Update(void) override;

	void UpdatePlay(void) override;
	void ProcessMove(void) override;


};

