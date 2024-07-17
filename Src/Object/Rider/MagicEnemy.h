#include "EnemyBase.h"

class MagicEnemy : public EnemyBase
{
public:

	MagicEnemy(const std::vector<std::shared_ptr<Bike>>& bikes,GameScene* gameScene, VECTOR loopStagePos, VECTOR localPos);

	//�p�����[�^�ݒ�		//override�㏑��
	void SetParam(void) override;
	void Update(void) override;

	void UpdatePlay(void) override;
	void ProcessMove(void) override;


};

