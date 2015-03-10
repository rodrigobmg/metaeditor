#include "linenumberwidget.h"

LineNumberWidget::LineNumberWidget(PlainEditor *editor) :
    QWidget(editor), m_editor(editor)
{
}

QSize LineNumberWidget::sizeHint() const
{
    if( m_editor )
    {
        return QSize(m_editor->lineNumberAreaWidth(), 0);
    }

    return QSize(0, 0);
}

void LineNumberWidget::paintEvent(QPaintEvent *event)
{
    if( m_editor )
    {
        m_editor->lineNumberAreaPaintEvent(event);
    }
}
