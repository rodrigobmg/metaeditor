#ifndef LINENUMBERWIDGET_H
#define LINENUMBERWIDGET_H

/**
* modulo LineNumberWidget
* autor  Rodrigo Monteiro 1313493
* descrição Utilitário para exibição das linhas contidas no editor
* versão
*   12/2014 1.0 Módulo inicial
*   12/2014 1.1 Módulo finalizado
**/

#include <QWidget>
#include "../plaineditor.h"

class LineNumberWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LineNumberWidget(PlainEditor *editor);
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    PlainEditor* m_editor;
};

#endif // LINENUMBERWIDGET_H
