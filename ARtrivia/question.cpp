#include "question.hpp"
#include "ui_question.h"
#include <QFont>

Question *Question::instance = NULL;

Question::Question( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::Question ),
    answerTimerCounter( 0 ),
    answerTimer( new QTimer( this ) ),
    resultTimer( new QTimer( this ) )
{
    ui->setupUi( this );

    ui->scrollAreaWidgetContents->setImage( "background.jpg", FILL );

    connect( answerTimer,
             SIGNAL( timeout() ),
             SLOT( answerTimerTimeout() ) );

    connect( resultTimer,
             SIGNAL( timeout() ),
             SLOT( resultTimerTimeout() ) );

    connect( DataManager::getInstance(),
             SIGNAL( readyAnswer( bool, int, int, int ) ),
             SLOT( showAnswer( bool, int, int, int ) ) );

    ui->scrollArea->setFrameShape( QFrame::NoFrame );

    QPalette palette = this->palette();
    palette.setColor( QPalette::WindowText, QColor( 255, 255, 255 ) );
//    palette.setColor( QPalette::Background, QColor( "#35234b" ) );
    palette.setColor( QPalette::Background, QColor( "#ACB353" ) );
    this->setPalette( palette );

    QPalette scrollAreaPalette = ui->scrollArea->palette();
    scrollAreaPalette.setColor( QPalette::WindowText, QColor( 255, 255, 255 ) );
//    scrollAreaPalette.setColor( QPalette::Background, QColor( "#35234b" ) );
    scrollAreaPalette.setColor( QPalette::Background, QColor( "#ACB353" ) );
    ui->scrollArea->setPalette( scrollAreaPalette );

    QPalette scrollAreaWidget = ui->scrollAreaWidgetContents->palette();
    scrollAreaWidget.setColor( QPalette::WindowText, QColor( 0, 0, 0 ) );
    ui->scrollAreaWidgetContents->setPalette( scrollAreaWidget );

    QFont serifFont("Times", this->width()/10, QFont::Bold);

    ui->questionLabel->setFont(serifFont);
}

Question *Question::getInstance()
{
    if( !instance )
    {
        instance = new Question();
    }
    return instance;
}

Question::~Question()
{
    delete ui;
    delete instance;
}

int Question::getTeamId() const
{
    return teamId;
}

void Question::setTeamId(int value)
{
    teamId = value;
}

int Question::getCurrentQuestionCategoryId() const
{
    return currentQuestionCategoryId;
}

void Question::setCurrentQuestionCategoryId( int value )
{
    currentQuestionCategoryId = value;
}

int Question::getCurrentQuestionId() const
{
    return currentQuestionId;
}

void Question::setCurrentQuestionId( int value )
{
    currentQuestionId = value;
}

int Question::getAnswerTimerCounter() const
{
    return answerTimerCounter;
}

void Question::setAnswerTimerCounter(int value)
{
    answerTimerCounter = value;
}

void Question::initQuestion( QString questionText,
                             QVector< QStringList > answers,
                             int teamId,
                             int categoryId,
                             QString categoryName,
                             int questionId )
{
    this->setTeamId( teamId );
    this->setCurrentQuestionCategoryId( categoryId );
    this->setCurrentQuestionId( questionId );

    this->clearScrollArea();

    ui->categoryLabel->setText( categoryName );
    ui->questionLabel->setText( questionText );

    for( int i = 0; i < answers.size(); i++ )
    {
        IndexedButton *answer = new IndexedButton( answers.at( i ).at( 1 ), i + 1 );
        ui->scrollAreaLayout->addWidget( answer, i + 1, 0, 1, 1, Qt::AlignCenter );

        connect( answer,
                 SIGNAL( pressedWithIndex( int ) ),
                 SLOT( answerPressed( int ) ) );
    }

    this->initTimer();
}

void Question::clearScrollArea()
{
    QList< QPushButton* > buttonsList = ui->scrollArea->findChildren< QPushButton* >();
    qDeleteAll( buttonsList );

    ui->questionLabel->clear();
}

void Question::initTimer()
{
    this->setAnswerTimerCounter( 15 );
    ui->counterLabel->setText( QString::number( this->getAnswerTimerCounter() ) );
    answerTimer->start( 1000 );
}

void Question::answerTimerTimeout()
{
    this->setAnswerTimerCounter( this->getAnswerTimerCounter() - 1 );
    ui->counterLabel->setText( QString::number( this->getAnswerTimerCounter() ) );

    if( !this->getAnswerTimerCounter() )
    {
        this->answerPressed( -1 );
        answerTimer->stop();
        return;
    }
}

void Question::resultTimerTimeout()
{
    resultTimer->stop();
    emit activateGame();
}

void Question::answerPressed( int index )
{
    answerTimer->stop();

    DataManager::getInstance()->requestAnswer( this->getTeamId(),
                                               this->getCurrentQuestionCategoryId(),
                                               this->getCurrentQuestionId(),
                                               index );

    ui->scrollArea->setEnabled( false );
}

void Question::showAnswer( bool correct, int updatedScore, int ranking, int numberOfTeams )
{
    this->clearScrollArea();

    ui->counterLabel->clear();

    ui->scrollArea->setEnabled( true );

    if( correct )
    {
        ui->questionLabel->setText( "Correcto, 100 puntos" );
    }
    else
    {
        ui->questionLabel->setText( "Incorrecto" );
    }

    emit questionFinished( updatedScore, ranking, numberOfTeams );

    resultTimer->start( 3000 );
}
