#include "EnemyBase.h"

class ShortDisEnemy : public EnemyBase
{
public:

	ShortDisEnemy(const std::vector<std::shared_ptr<Bike>>& bikes, VECTOR loopStagePos, VECTOR localPos);

	//パラメータ設定		//override上書き
	void SetParam(void) override;
	void Update(void) override;

	void UpdatePlay(void) override;
	void ProcessMove(void) override;


};

