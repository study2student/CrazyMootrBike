#include "EnemyBase.h"

class BombEnemy : public EnemyBase
{
public:

	BombEnemy();

	//�p�����[�^�ݒ�		//override�㏑��
	void SetParam(void) override;
	void Update(void) override;

	void UpdatePlay(void) override;
	void ProcessMove(void) override;


};

