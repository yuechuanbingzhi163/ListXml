#ifndef __UIANIMATION_H__
#define __UIANIMATION_H__

#pragma once
#include <vector>

namespace DuiLib {

	class UILIB_API IUIAnimation
	{
	public:
		virtual ~IUIAnimation() { NULL; }
	
		virtual BOOL StartAnimation(int nElapse, int nTotalFrame, int nAnimationID = 0, BOOL bLoop = FALSE) = 0;
		virtual VOID StopAnimation(int nAnimationID = 0) = 0;
		virtual BOOL IsAnimationRunning(int nAnimationID) = 0;
		virtual int GetCurrentFrame(int nAnimationID = 0) = 0;
		virtual int GetTotalFram(int nAnimationID = 0) = 0;
		virtual BOOL SetCurrentFrame(int nFrame, int nAnimationID = 0) = 0;	
		virtual VOID OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID) = 0;
		virtual VOID OnAnimationStart(int nAnimationID, BOOL bFirstLoop) = 0;
		virtual VOID OnAnimationStop(int nAnimationID) = 0;

		virtual VOID OnAnimationElapse(int nAnimationID) = 0;
	};

	class UILIB_API CAnimationData
	{
	public:
		CAnimationData(int nElipse, int nFrame, int nID, BOOL bLoop)
		{
			//是否第一次循环;
			m_bFirstLoop = TRUE;
			//当前值;
			m_nCurFrame = 0;
			//触发间隔;
			m_nElapse = nElipse;
			//目标值;
			m_nTotalFrame = nFrame;
			//是否循环;
			m_bLoop = bLoop;
			//动画ID;
			m_nAnimationID = nID;
		}

	//protected:
	public:
		int m_nAnimationID;
		int m_nElapse;

		int m_nTotalFrame;
		int m_nCurFrame;

		BOOL m_bLoop;
		BOOL m_bFirstLoop;
	};

	class UILIB_API CUIAnimation: public IUIAnimation
	{
	public:
		CUIAnimation();
		CUIAnimation(CControlUI* pOwner);
		~CUIAnimation();
		void SetOwner(CControlUI *pOwner);
		virtual BOOL StartAnimation(int nElapse, int nTotalFrame, int nAnimationID = 0, BOOL bLoop = FALSE);
		virtual VOID StopAnimation(int nAnimationID = 0);
		virtual BOOL IsAnimationRunning(int nAnimationID);
		virtual int GetCurrentFrame(int nAnimationID = 0);
		virtual int GetTotalFram(int nAnimationID = 0);
		virtual BOOL SetCurrentFrame(int nFrame, int nAnimationID = 0);

		virtual VOID OnAnimationStart(int nAnimationID, BOOL bFirstLoop) {};
		virtual VOID OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID) {};
		virtual VOID OnAnimationStop(int nAnimationID){};


		virtual VOID OnAnimationElapse(int nAnimationID);

	protected:
		CAnimationData* GetAnimationDataByID(int nAnimationID);

	protected:
		CControlUI* m_pControl;
		std::vector<CAnimationData*> m_arAnimations;
	};

} // namespace DuiLib

#endif // __UIANIMATION_H__