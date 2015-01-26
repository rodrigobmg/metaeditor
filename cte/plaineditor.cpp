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
#ifdef __LOG_TEST__
    logFile.write("[PlainEditor] ctor");
#endif
    m_lineNumberArea = new LineNumberWidget(this);
    m_dataman = new LineManager;

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

PlainEditor::~PlainEditor()
{
#ifdef __LOG_TEST__
    logFile.write("[PlainEditor] dtor");
#endif
    if( m_dataman )
        delete m_dataman;

    if( m_lineNumberArea )
        delete m_lineNumberArea;
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

    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
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

bool PlainEditor::eventFilter(QObject * obj, QEvent * event)
{
    if (event->type() == QEvent::KeyPress)
    {
#ifdef __LOG_TEST__
        logFile.write("[PlainEditor] KeyPress detected");
#endif
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if( keyEvent->modifiers().testFlag(Qt::ControlModifier) )
        {
            return QObject::eventFilter(obj, event);
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
                if(m_dataman->isEditable(columnNumber(), currentLine()) == false)
                {
                    return true;
                }
                return QObject::eventFilter(obj, event);
#ifdef __LOG_TEST__
            case Qt::Key_Escape:
                logFile.write("[PlainEditor] Dumping lines");
                m_dataman->dump();
                return true;

            case Qt::Key_F1:
                logFile.write("[PlainEditor] Inserting a new column");
                this->insertPlainText(" ");
                return true;

            case Qt::Key_F2:
                logFile.write("[PlainEditor] Inserting a title");
                m_dataman->addTitle("Nome da base de software:", columnNumber(), currentLine());
                return true;

            case Qt::Key_F3:
                logFile.write("[PlainEditor] Inserting a name");
                m_dataman->addName("EDT Nome para busca", columnNumber(), currentLine());
                return true;
#endif

            case Qt::Key_Backspace:
            {
                if(textCursor().selectionStart() == textCursor().selectionEnd())
                {
                    if(m_dataman->isEditable(columnNumber(), currentLine()) == false)
                    {
                        return true;
                    }
#ifdef __LOG_TEST__
                    logFile.write("[PlainEditor] Deleting character");
#endif
                    m_dataman->removeCharacter(columnNumber(), currentLine());
                    return QObject::eventFilter(obj, event);
                }

                return true;
            }

            case Qt::Key_Delete:
            {

                if(textCursor().selectionStart() == textCursor().selectionEnd())
                {
                    if(m_dataman->isEditable(columnNumber() + 1, currentLine()) == false)
                    {
                        return true;
                    }
#ifdef __LOG_TEST__
                    logFile.write("[PlainEditor] Deleting a character");
#endif
                    m_dataman->removeCharacter(columnNumber() + 1, currentLine());
                    return QObject::eventFilter(obj, event);
                }

                return true;
            }

            default:
            {
#ifdef __LOG_TEST__
                logFile.write("[PlainEditor] Inserting a new character");
#endif
                if(m_dataman->eventFilter(columnNumber(), currentLine(), keyEvent->text().at(0).toLatin1()) == false)
                {
                    return true;
                }

                return QObject::eventFilter(obj, event);
                //return false;
            }
        }// switch( keyEvent->key() )
    }// if (event->type() == QEvent::KeyPress)

    //Continua a execução normalmente
    return QPlainTextEdit::eventFilter(obj, event);
}

void PlainEditor::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        if(e->modifiers() == Qt::ControlModifier)
        {
            QMessageBox::information(this, "Teste", "Control + Click", QMessageBox::Ok);
        }
    }


    return QPlainTextEdit::mouseMoveEvent(e);
}

void PlainEditor::moveToLine(unsigned int lineNumber)
{
#ifdef __LOG_TEST__
    logFile.write("[PlainEditor] Move to line [%d]", lineNumber);
#endif
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
#ifdef __LOG_TEST__
    logFile.write("[PlainEditor] Moving to next line");
#endif
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
#ifdef __LOG_TEST__
    logFile.write("[PlainEditor] recovering the line count");
#endif
    return this->document()->lineCount();
}

int PlainEditor::columnNumber()
{
    int columnNumber = this->textCursor().columnNumber();
    return columnNumber < 0 ? 0 : columnNumber;
}

bool PlainEditor::isEditable()
{
    return m_dataman->isEditable(columnNumber(), currentLine());
}

const QString PlainEditor::objectType()
{
    return QString::fromStdString(m_dataman->type(columnNumber(), currentLine()));
}

/***
 * IEditor Interface implementation
 * @Author: Rodrigo Monteiro
 ***/
int PlainEditor::currentLine()
{
    return textCursor().blockNumber();
}

int PlainEditor::currentColumn()
{
#ifdef __LOG_TEST__
    logFile.write("[PlainEditor] get the current column");
#endif
    return columnNumber();
}

void PlainEditor::jumpLine()
{
#ifdef __LOG_TEST__
    logFile.write("[PlainEditor] jump one line");
#endif
    moveToNextLine();
}

void PlainEditor::addTitle(const char * str)
{
#ifdef __LOG_TEST__
    logFile.write("[PlainEditor] insert the title [%s] at the document", str);
#endif

    int col = 0;
    if(columnNumber() > 1)
    {
        col = columnNumber() + 2;
    }

    m_dataman->addTitle(str, col, currentLine());
    this->textCursor().insertText(str);
}

void PlainEditor::addName(const char * str)
{
#ifdef __LOG_TEST__
    logFile.write("[PlainEditor] insert the name [%s] at the document", str);
#endif

    int col = 0;
    if(columnNumber() > 1)
    {
        col = columnNumber() + 2;
    }

    m_dataman->addName(str, col, currentLine());
    this->textCursor().insertText(str);
}
#ifdef __LOG_TEST__
int PlainEditor::structureLineCount()
{

    logFile.write("[PlainEditor] structure line count called");

    return (int)m_dataman->lineCount();
}

void PlainEditor::writeText(const char *str)
{
    logFile.write("[PlainEditor] insert a text at document");
    this->insertPlainText(QString::fromLatin1(str));
}
#endif
