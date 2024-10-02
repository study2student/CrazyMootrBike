#include "CoinBase.h"

class CopperCoin : public CoinBase
{

public:

	CopperCoin(const std::vector<std::shared_ptr<Bike>>& bikes,GameScene* gameScene, VECTOR loopStagePos, VECTOR localPos);

	//パラメータ設定		//override上書き
	void SetParam(void) override;
	void Update(void) override;

	void UpdatePlay(void) override;
	void ProcessMove(void) override;


};

