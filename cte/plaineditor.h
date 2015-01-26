#ifndef PLAINEDITOR_H
#define PLAINEDITOR_H

/**
* @modulo PlainEditor
* @autor  Rodrigo Monteiro 1313493
* @descrição Editor de textos dirigido a estrutura de dados
* @versão
*   12/2014 1.0 Módulo inicial
*   12/2014 1.1 Módulo finalizado
**/

#include <QPlainTextEdit>
#include <QPair>
#include <QString>
#include "Interfaces/IEditor.h"
#include "Data/dataman.h"

/**
 * @brief The PlainEditor class
 */
class PlainEditor : public QPlainTextEdit, public IEditor
{
    Q_OBJECT
public:
    /**
     * @brief PlainEditor ctor
     * @param parent widget
     */
    explicit PlainEditor(QWidget *parent = nullptr);
    /**
     * @brief PlainEditor dtor
     */
    ~PlainEditor();

    /**
     * @brief defines the area of editor to paint the line number
     * @param the event
     */
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    /**
     * @brief calculation of width to be painted
     * @return the width
     */
    int lineNumberAreaWidth();
    /**
     * @brief eventFilter Filter the events which happens in editor
     * @param obj the object
     * @param event the raised event
     * @return true if ok
     */
    virtual bool eventFilter(QObject * obj, QEvent * event);
    /**
     * @brief mousePressEvent Filter the mouse events that happens in editor
     * @param e The mouse event
     */
    virtual void mousePressEvent(QMouseEvent *e);
    //Util
    /**
     * @brief moveToLine Move the cursor to given line
     * @param lineNumber the line
     */
    void moveToLine(unsigned int lineNumber);
    /**
     * @brief moveToNextLine Move the cursor to the line below
     */
    void moveToNextLine();
    /**
     * @brief lineCount get the number of lines
     * @return the number of lines
     */
    int lineCount();
    /**
     * @brief columnNumber get the number of columns of current line
     * @return the column number
     */
    int columnNumber();
    /**
     * @brief isEditable verify if the current position is editable or not
     * @return true if is possible to edit
     */
    bool isEditable();
    /**
     * @brief objectType The type of object in current position
     * @return string representation of type
     */
    const QString objectType();

    //Interface implementation
    /**
     * @brief currentLine @see{IEditor::currentLine}
     */
    virtual int currentLine();
    /**
     * @brief currentColumn @see{IEditor::currentColumn}
     */
    virtual int currentColumn();
    /**
     * @brief jumpLine @see{IEditor::jumpLine}
     */
    virtual void jumpLine();
    /**
     * @brief addTitle @see{IEditor::addTitle}
     */
    virtual void addTitle(const char * str);
    /**
     * @brief addName @see{IEditor::addName}
     */
    virtual void addName(const char* str);
#ifdef __LOG_TEST__
    /**
     * @brief structureLineCount @see{IEditor::structureLineCount}
     */
    virtual int structureLineCount();
    /**
     * @brief writeText @see{IEditor::writeText}
     */
    virtual void writeText(const char * str);
#endif
protected:
    /**
     * @brief resizeEvent handles the event of resize editor
     * @param e
     */
    virtual void resizeEvent(QResizeEvent *e);

private slots:
    /**
     * @brief updateLineNumberAreaWidth update the line number area
     * @param newBlockCount
     */
    void updateLineNumberAreaWidth(int newBlockCount);
    /**
     * @brief highlightCurrentLine
     */
    void highlightCurrentLine();
    /**
     * @brief updateLineNumberArea
     * @param rect the new rectangle area
     * @param dy direction y
     */
    void updateLineNumberArea(const QRect& rect, int dy);

private:
    /**
     * @brief m_lineNumberArea handles the line number area
     */
    QWidget *m_lineNumberArea;
    /**
     * @brief m_countCache Used by line number area for better performance
     */
    QPair<int, int> m_countCache;
    /**
     * @brief Manager of structure
     */
    LineManager* m_dataman;
};

#endif // PLAINEDITOR_H
