#include "linenumberwidget.h"

LineNumberWidget::LineNumberWidget(PlainEditor *editor) :
    QWidget(editor), m_editor(editor)
{
}

QSize LineNumberWidget::sizeHint() const
{
    return QSize(m_editor->lineNumberAreaWidth(), 0);
}

void LineNumberWidget::paintEvent(QPaintEvent *event)
{
    m_editor->lineNumberAreaPaintEvent(event);
}
