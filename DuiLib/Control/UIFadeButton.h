#ifndef __UIFADEBUTTON_H__
#define __UIFADEBUTTON_H__

#pragma once
#include "UIAnimation.h"


namespace DuiLib {
	
	class UILIB_API CFadeButtonUI : public CButtonUI, public CUIAnimation
	{
	public:
		CFadeButtonUI();
		virtual ~CFadeButtonUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void SetNormalImage(LPCTSTR pStrImage);

		void SetOutEvent(BOOL bOutEvent){m_bOutEvent = bOutEvent;}
		BOOL GetOutEvent(){return m_bOutEvent;}

		void DoEvent(TEventUI& event);
		void OnTimer( int nTimerID );
		void PaintStatusImage(HDC hDC);

		virtual void OnAnimationStart(INT nAnimationID, BOOL bFirstLoop) {}
		virtual void OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID);
		virtual void OnAnimationStop(INT nAnimationID) {}
		
	protected:
		CDuiString m_sOldImage;
		CDuiString m_sNewImage;
		CDuiString m_sLastImage;
		BYTE       m_bFadeAlpha;
		BOOL       m_bMouseHove;
		BOOL       m_bMouseLeave;
		BOOL	   m_bButtonHotState;
		BOOL m_bOutEvent;
		enum{
			FADE_IN_ID			= 8,
			FADE_OUT_ID			= 9,

			FADE_ELLAPSE		= 10,
			FADE_FRAME_COUNT	= 30,
		};
	};

} // namespace UiLib

#endif // __UIFADEBUTTON_H__