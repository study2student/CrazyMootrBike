#include "CoinBase.h"

class SilverCoin : public CoinBase
{
public:

	SilverCoin(const std::vector<std::shared_ptr<Bike>>& bikes, GameScene* gameScene, VECTOR loopStagePos, VECTOR localPos);

private:

	//ƒpƒ‰ƒ[ƒ^İ’è
	void SetParam(void) override;

};

