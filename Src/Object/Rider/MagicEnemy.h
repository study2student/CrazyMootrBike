#include "EnemyBase.h"

class MagicEnemy : public EnemyBase
{
public:

	MagicEnemy(Bike* bike, VECTOR loopStagePos, VECTOR localPos);

	//�p�����[�^�ݒ�		//override�㏑��
	void SetParam(void) override;
	void Update(void) override;

	void UpdatePlay(void) override;
	void ProcessMove(void) override;


};

