#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QTextCodec>
#include <QTextDocumentWriter>
#include <QTextCharFormat>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void textoModificado();
    void textoSelecionado(const QTextCharFormat &formato);

    void on_actionNegrito_triggered();
    void on_actionIt_lico_triggered();

    void on_actionAbrir_triggered();

    void on_actionNovo_triggered();

    void on_actionSalvar_triggered();

    void on_actionSalvar_como_triggered();

    void on_actionSublinhado_triggered();

private:
    Ui::MainWindow *ui;
    QString arqNome;

    void abrirArquivo();
    void novoArquivo();
    void salvarArquivo();
    void defTituloJanela();
    void fonteAlterada(const QFont &f);
    void setaCaracteristica(const QTextCharFormat &f);
};

#endif // MAINWINDOW_H
