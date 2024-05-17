#pragma once

#pragma once
#include <map>
#include "Common/Transform.h"
#include "ActorBase.h"
class null
{
	
	public:


		// 状態
		enum class STATE
		{
			NONE,
			IDLE,
			MAKE,
			BACK
		};

		// コンストラクタ
		null();

		// デストラクタ
		~null(void);

		void Init(void) ;
		void Update(void) ;
		void Draw(void) ;
		void Destroy(void);

		VECTOR GetPos(void);

	private:

		// 状態管理
		STATE state_;

		// 状態遷移
		void ChangeState(STATE state);
		void ChangeStateNone(void);
		void ChangeStateIdle(void);
		void ChangeStateMake(void);
		void ChangeStateBack(void);

		// 更新ステップ
		void UpdateNone(void);
		void UpdateIdle(void);
		void UpdateMake(void);
		void UpdateBack(void);
};

