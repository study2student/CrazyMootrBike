#include "EnemyBase.h"

class ShortDisEnemy : public EnemyBase
{
public:

	ShortDisEnemy();

	//�p�����[�^�ݒ�		//override�㏑��
	void SetParam(void) override;
	void Update(void) override;

	void UpdatePlay(void) override;
	void ProcessMove(void) override;


};

