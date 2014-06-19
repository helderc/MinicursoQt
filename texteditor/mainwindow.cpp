#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Barra de Titulo da janela.
    defTituloJanela();

    // Toda vez que o texto for modificado, dispara o sinal para
    // sinalizar na barra de título
    connect(ui->editor, SIGNAL(textChanged()), this, SLOT(textoModificado()));

    // sinal disparado quando a formatacao do espaco/caractere(s) sob o cursor
    // sofre alguma alteracao
    connect(ui->editor, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SLOT(textoSelecionado(QTextCharFormat)));


    // Connectando as funcoes do menu com os slots do editor
    connect(ui->actionRecortar, SIGNAL(triggered()), ui->editor, SLOT(cut()));
    connect(ui->actionCopiar, SIGNAL(triggered()), ui->editor, SLOT(copy()));
    connect(ui->actionColar, SIGNAL(triggered()), ui->editor, SLOT(paste()));

    // Habilita/Desabilita os botões de Copiar e Recortar, baseando-se na copia.
    connect(ui->editor, SIGNAL(copyAvailable(bool)), ui->actionCopiar, SLOT(setEnabled(bool)));
    connect(ui->editor, SIGNAL(copyAvailable(bool)), ui->actionRecortar, SLOT(setEnabled(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::textoModificado()
{
    this->setWindowModified(true);
}

void MainWindow::textoSelecionado(const QTextCharFormat &formato)
{
    fonteAlterada(formato.font());
}

void MainWindow::on_actionNegrito_triggered()
{
    QTextCharFormat fmt;

    // Negrito eh o estilo da fonte (diferente do italico): BOLD/NORMAL
    fmt.setFontWeight(ui->actionNegrito->isChecked() ?
                          QFont::Bold : QFont::Normal);

    setaCaracteristica(fmt);
}

void MainWindow::on_actionIt_lico_triggered()
{
    QTextCharFormat fmt;

    // Italico eh soh TRUE/FALSE
    fmt.setFontItalic(ui->actionIt_lico->isChecked());

    setaCaracteristica(fmt);
}

void MainWindow::on_actionAbrir_triggered()
{
    abrirArquivo();
}

void MainWindow::abrirArquivo()
{
    arqNome = QFileDialog::getOpenFileName(this,
                                           "Abrir arquivo...",
                                           "",
                                           "Rich Text Format (*.htm *.html);;All Files (*)");

    if (!arqNome.isEmpty()) {
        // Interface para Leitura/Escrita de arquivos
        QFile arq(arqNome);

        // Abre o arquivo para Leitura e Escrita
        arq.open(QFile::ReadWrite);

        // Lê todo o arquivo de texto retornando um Array de Bytes
        QByteArray data = arq.readAll();

        // Tenta verificar se os dados correspondem a um arquivo HTML
        // retornando a respectiva codificação
        QTextCodec *codec = Qt::codecForHtml(data);

        // Converte o Array de Bytes para String
        QString str = codec->toUnicode(data);

        // Mostra o arquivo no editor
        ui->editor->setHtml(str);

        // Caminho do arquivo a barra de titulo da janela
        defTituloJanela();

        this->setWindowModified(false);
    }
}

void MainWindow::novoArquivo()
{
    // Nome do arquivo Vazio
    arqNome.clear();

    // Limpa a area de edicao
    ui->editor->clear();

    // Titulo da Janela
    defTituloJanela();

    // Nenhuma modificacao
    this->setWindowModified(false);
}

void MainWindow::salvarArquivo()
{
    // Se o nome do arquivo eh vazio, entao: "salvar como"...
    if (arqNome.isEmpty()) {
        arqNome = QFileDialog::getSaveFileName(this,
                                               "Salvar como...",
                                               "arquivo",
                                               "HTML (*.htm *.html);;All Files (*)");
        // Caso "cancele" a janela de salvar
        if (arqNome.isEmpty())
            return;

        // verifica se o usuario digitou a extensao
        if (!(arqNome.endsWith(".htm", Qt::CaseInsensitive) ||
              arqNome.endsWith(".html", Qt::CaseInsensitive)))
            arqNome += ".htm";

        // Agora o arquivo possui nome
        salvarArquivo();

        return;
    }
    // ...senao salva normal.
    else {
        QTextDocumentWriter writer(arqNome);

        // Grava o arquivo (ou nao)
        bool ok = writer.write(ui->editor->document());

        // Salvou, entao nao ha modificacoes!
        if (ok)
            this->setWindowModified(false);
    }

    defTituloJanela();
}

void MainWindow::defTituloJanela()
{
    if (arqNome.isEmpty())
        // Barra de Titulo da janela.
        // [*] para definir a flag de modificacoes
        this->setWindowTitle("Editor[*]");
    else
        this->setWindowTitle(QString("%1[*] - %2") \
                             .arg(QFileInfo(arqNome).fileName()) \
                             .arg(arqNome));
}

void MainWindow::fonteAlterada(const QFont &f)
{
    ui->actionNegrito->setChecked(f.bold());
    ui->actionIt_lico->setChecked(f.italic());
    ui->actionSublinhado->setChecked(f.underline());
}

void MainWindow::setaCaracteristica(const QTextCharFormat &f)
{
    QTextCursor cursor = ui->editor->textCursor();

    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);

    cursor.mergeCharFormat(f);
    ui->editor->mergeCurrentCharFormat(f);
}

void MainWindow::on_actionNovo_triggered()
{
    novoArquivo();
}

void MainWindow::on_actionSalvar_triggered()
{
    salvarArquivo();
}

void MainWindow::on_actionSalvar_como_triggered()
{
    arqNome.clear();
    salvarArquivo();
}

void MainWindow::on_actionSublinhado_triggered()
{
    QTextCharFormat fmt;

    // Sublinhado, apenas TRUE/FALSE
    fmt.setFontUnderline(ui->actionSublinhado->isChecked());

    setaCaracteristica(fmt);
}
