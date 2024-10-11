#include "CoinBase.h"

class GoldCoin : public CoinBase
{
public:

	GoldCoin(const std::vector<std::shared_ptr<Bike>>& bikes, GameScene* gameScene,VECTOR loopStagePos, VECTOR localPos);

	//ƒpƒ‰ƒ[ƒ^İ’è
	void SetParam(void) override;
	void Update(void) override;

	void UpdatePlay(void) override;
	void ProcessMove(void) override;


};

