#include "EnemyBase.h"

class ShortDisEnemy : public EnemyBase
{
public:

	ShortDisEnemy(Bike* bike, VECTOR loopStagePos, VECTOR localPos);

	//�p�����[�^�ݒ�		//override�㏑��
	void SetParam(void) override;
	void Update(void) override;

	void UpdatePlay(void) override;
	void ProcessMove(void) override;


};

