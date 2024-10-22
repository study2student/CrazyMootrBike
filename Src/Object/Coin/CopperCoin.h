#include "CoinBase.h"

class CopperCoin : public CoinBase
{

public:

	CopperCoin(const std::vector<std::shared_ptr<Bike>>& bikes,GameScene* gameScene, VECTOR loopStagePos, VECTOR localPos);

private:

	//ƒpƒ‰ƒ[ƒ^İ’è
	void SetParam(void) override;

};

