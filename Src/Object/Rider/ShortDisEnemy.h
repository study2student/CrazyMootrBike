#include "EnemyBase.h"

class ShortDisEnemy : public EnemyBase
{
public:

	ShortDisEnemy();

	//パラメータ設定		//override上書き
	void SetParam(void) override;
	void Update(void) override;

	void UpdatePlay(void) override;
	void ProcessMove(void) override;


};

