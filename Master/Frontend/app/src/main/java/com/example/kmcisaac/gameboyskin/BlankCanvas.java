package com.example.kmcisaac.gameboyskin;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.View;

/**
 * Created by KMcIsaac on 11/29/2017.
 */

public class BlankCanvas extends View {
    public BlankCanvas(Context context, AttributeSet attrs)
    {
        super(context,attrs);
        TypedArray a = context.getTheme().obtainStyledAttributes(
                attrs,
                R.styleable.BlankCanvas,
                0, 0);
        try {
            canvasText = a.getString(R.styleable.BlankCanvas_label);
        }
        finally{
            a.recycle();
        }
        mTextPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mTextPaint.setColor(0xffffffff);
        mTextPaint.setTextSize(mTextPaint.getTextSize()*4);

        mBackground = new Paint(Paint.ANTI_ALIAS_FLAG);
        mBackground.setColor(0xff000000);
    }
    private int midX;
    private int midY;
    private Rect canvasRect;
    private String canvasText;
    private Paint mTextPaint;
    private Paint mBackground;

    public void onSizeChanged(int w, int h, int oldW, int oldH)
    {
        midX=w/2;
        midY=h/2;

        canvasRect = new Rect();
        canvasRect.left=getPaddingLeft();
        canvasRect.right=w-getPaddingRight();
        canvasRect.top= getPaddingTop();
        canvasRect.bottom=h-getPaddingBottom();

    }
    public void onDraw(Canvas canvas)
    {
        canvas.drawRect(canvasRect,mBackground);
        float v = mTextPaint.measureText(canvasText);
        canvas.drawText(canvasText,midX-v/2,midY,mTextPaint);
    }
    public void setText(String text)
    {
        canvasText = text;
        invalidate();
    }
}
