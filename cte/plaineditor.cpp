#include "plaineditor.h"
#include "Utils/linenumberwidget.h"
#include "Utils/logfile.h"

#include <iostream>
#include <QMessageBox>
#include <QPainter>
#include <QTextBlock>

#ifdef __LOG_TEST__
static LogFile logFile("PlainEditor", "LogTest", true);
#endif

PlainEditor::PlainEditor(QWidget *parent) :
    QPlainTextEdit(parent)
{
    //m_lineNumberArea = new LineNumberWidget(this);
    m_lineMng = std::unique_ptr<LineManager>(new LineManager);
    readOnly(true);
    setCursorWidth(12);

    QPalette p = this->palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::gray);
    setPalette(p);
    //connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    //connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    //connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    //updateLineNumberAreaWidth(0);
    //highlightCurrentLine();
}

PlainEditor::PlainEditor(const std::string& scriptName, QWidget *parent)
{
}

PlainEditor::~PlainEditor()
{
    /*if( m_lineNumberArea )
    {
        delete m_lineNumberArea;
        m_lineNumberArea = nullptr;
    }*/
}

int PlainEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while( max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void PlainEditor::updateLineNumberAreaWidth(int /*newBlockCount*/)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void PlainEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
    {
        m_lineNumberArea->scroll(0, dy);
    }
    else if (m_countCache.first != blockCount() ||
             m_countCache.second != textCursor().block().lineCount())
    {
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
        m_countCache.first = blockCount();
        m_countCache.second = textCursor().block().lineCount();
    }

    if (rect.contains(viewport()->rect()))
    {
        updateLineNumberAreaWidth(0);
    }
}

void PlainEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    //QRect cr = contentsRect();
    //m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void PlainEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void PlainEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, m_lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void PlainEditor::readOnly(bool status)
{
    this->setReadOnly(status);
    if( status )
    {
        setTextInteractionFlags( this->textInteractionFlags() | Qt::TextSelectableByKeyboard);
    }
}

bool PlainEditor::eventFilter(QObject * obj, QEvent * event)
{
    if(this->m_lineMng->lineCount() <= 0)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if(keyEvent->key() == Qt::Key_1)
        {
            readOnly(true);
        }
        else if( keyEvent->key() == Qt::Key_2)
        {
            readOnly(false);
        }

        return QObject::eventFilter(obj, event);
    }

    unsigned int curColumn = columnNumber();
    unsigned int curLine = currentLine();

    curLine -= 1;
    std::cout << "Colnumber: " << curColumn << std::endl;
    if(m_lineMng->isField(curLine, curColumn) == false)
    {
        std::cout << "not field" << std::endl;
        return true;
    }

    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        std::cout << "key = " << keyEvent->key() << std::endl;

        if( keyEvent->modifiers().testFlag(Qt::ControlModifier))
        {
            return QObject::eventFilter(obj, event);
        }

        if( keyEvent->key() > 127 )
        {
            return false;
        }

        switch( keyEvent->key() )
        {
            case Qt::Key_CapsLock:
            case Qt::Key_Left:
            case Qt::Key_Down:
            case Qt::Key_Right:
            case Qt::Key_Up:
                return QObject::eventFilter(obj, event);

            case Qt::Key_Enter:
            case Qt::Key_Return:
                if(m_lineMng->isEditable(curColumn, curLine) == false)
                {
                    return false;
                }
                return QObject::eventFilter(obj, event);

            case Qt::Key_Backspace:
            {
                std::cout << "Colnumber: " << curColumn << std::endl;
                if(textCursor().selectionStart() == textCursor().selectionEnd())
                {
                    if(m_lineMng->isEditable(curColumn, curLine) == false)
                    {
                        std::cout << "Backspace" << std::endl;
                        return false;
                    }
                    m_lineMng->removeCharacter(curColumn, curLine);
                    return QObject::eventFilter(obj, event);
                }

                return false;
            }

            case Qt::Key_Delete:
            {

                if(textCursor().selectionStart() == textCursor().selectionEnd())
                {
                    if(m_lineMng->isEditable(curColumn + 1, curLine) == false)
                    {
                        return false;
                    }

                    m_lineMng->removeCharacter(curColumn + 1, curLine);
                    return QObject::eventFilter(obj, event);
                }

                return true;
            }

            default:
            {
                if(m_lineMng->eventFilter(curColumn, curLine, keyEvent->text().at(0).toLatin1()) == false)
                {
                    return false;
                }

                return QObject::eventFilter(obj, event);
            }
        }
    }

    //Continua a execução normalmente
    return QPlainTextEdit::eventFilter(obj, event);
}

void PlainEditor::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        if(e->modifiers() == Qt::ControlModifier)
        {
            emit openNewEditor();
        }
    }

    return QPlainTextEdit::mouseMoveEvent(e);
}

void PlainEditor::moveToLine(unsigned int lineNumber)
{
    QTextBlock block = this->document()->findBlockByLineNumber(lineNumber);

    if(block.isValid() == false)
    {
        unsigned int lines = this->document()->lineCount();

        for( unsigned int i = lines; i <= lineNumber; i++)
        {
            textCursor().insertBlock();
        }

        block = this->document()->findBlockByLineNumber(lineNumber);
    }

    QTextCursor cursor(block);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 0);
    setTextCursor(cursor);
    centerCursor();
    setFocus();
}

void PlainEditor::moveToNextLine()
{
    int nextLine = this->textCursor().blockNumber() + 1;

    QTextBlock block = this->document()->findBlockByNumber(nextLine);

    if(block.isValid() == false)
    {
        textCursor().insertBlock();
        block = this->document()->findBlockByNumber(nextLine);
    }

    QTextCursor cursor(block);
    cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 0);
    setTextCursor(cursor);
    centerCursor();
    setFocus();
}

int PlainEditor::lineCount()
{
    return this->document()->lineCount();
}

int PlainEditor::columnNumber()
{
    if(this->isVisible())
    {
        int columnNumber = this->textCursor().columnNumber();
        return columnNumber < 0 ? 0 : columnNumber;
    }

    return 0;
}

bool PlainEditor::isEditable()
{
    if( m_lineMng != nullptr )
    {
        return m_lineMng->isEditable(columnNumber(), currentLine() - 1);
    }

    return false;
}

const QString PlainEditor::objectType()
{
    if( m_lineMng != nullptr )
    {
        return QString::fromStdString(m_lineMng->type(columnNumber(), currentLine() - 1));
    }

    return "Error";
}

lua_State* PlainEditor::luaState()
{
    return m_luaEnv.state();
}

/***
 * IEditor Interface implementation
 * @Author: Rodrigo Monteiro
 ***/
int PlainEditor::currentLine()
{
    return textCursor().blockNumber() + 1;
}

int PlainEditor::currentColumn()
{
    return columnNumber();
}

void PlainEditor::jumpLine()
{
    moveToNextLine();
}

void PlainEditor::moveCursor(int many)
{
    QTextCursor cursor = this->textCursor();
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, many);
    setTextCursor(cursor);
}

void PlainEditor::printStructure()
{
    QString wholeText;

    for( int i = 0; i < m_lineMng->count(); i++ )
    {
        //wholeText.append(QString::fromStdString(m_));
    }
}

void PlainEditor::createField(int type, const std::string &text, std::string *fieldCallback)
{
    int col = columnNumber();

    m_lineMng->createField(static_cast<FieldType>(type), text, fieldCallback, col, currentLine() - 1);

    QString fmtText = QString::fromStdString(text);
    switch(type)
    {
        case Title:
        {
            fmtText.prepend("<font color=\"dark green\">").append("<\\font>");
        }
        break;
    case Text:
    case Name:
        {
            fmtText.prepend("<a href=#><font color=\"white\">").append("<\\font><\\a>");
        }
        break;
    }

    this->textCursor().insertHtml(fmtText);
}
