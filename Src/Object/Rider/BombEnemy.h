#include "EnemyBase.h"

class BombEnemy : public EnemyBase
{
public:

	BombEnemy(Bike* bike, VECTOR loopStagePos, VECTOR localPos);

	//�p�����[�^�ݒ�		//override�㏑��
	void SetParam(void) override;
	void Update(void) override;

	void UpdatePlay(void) override;
	void ProcessMove(void) override;


};

