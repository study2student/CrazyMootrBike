#pragma once

#pragma once
#include <map>
#include "Common/Transform.h"
#include "ActorBase.h"
class null
{
	
	public:


		// ���
		enum class STATE
		{
			NONE,
			IDLE,
			MAKE,
			BACK
		};

		// �R���X�g���N�^
		null();

		// �f�X�g���N�^
		~null(void);

		void Init(void) ;
		void Update(void) ;
		void Draw(void) ;
		void Destroy(void);

		VECTOR GetPos(void);

	private:

		// ��ԊǗ�
		STATE state_;

		// ��ԑJ��
		void ChangeState(STATE state);
		void ChangeStateNone(void);
		void ChangeStateIdle(void);
		void ChangeStateMake(void);
		void ChangeStateBack(void);

		// �X�V�X�e�b�v
		void UpdateNone(void);
		void UpdateIdle(void);
		void UpdateMake(void);
		void UpdateBack(void);
};

