#include "CoinBase.h"

class GoldCoin : public CoinBase
{
public:

	GoldCoin(const std::vector<std::shared_ptr<Bike>>& bikes, GameScene* gameScene,VECTOR loopStagePos, VECTOR localPos);

private:

	//ƒpƒ‰ƒ[ƒ^İ’è
	void SetParam(void) override;

};

