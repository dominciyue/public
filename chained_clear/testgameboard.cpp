#include <QtTest>
#include "gameboard.h"

class TestGameBoard : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testActivateBlock();
    void testFindPath();
    void testIsGameFinished();
    void testHasMatchingPairs();
    void testActivateBlockBoundary();
    void testFindPathBoundary();
    void testGameBoardEdges();
    void testLargeGameBoard();
    void testEmptyGameBoard();
    void testFullGameBoard();
    void testSingleBlockRemaining();
private:
    GameBoard *gameBoard;
};

void TestGameBoard::initTestCase()
{
    gameBoard = new GameBoard(nullptr, false); // 创建单人模式的游戏板
}

void TestGameBoard::cleanupTestCase()
{
    delete gameBoard;
}

void TestGameBoard::testActivateBlock()
{
    // 设置一个已知的游戏状态
    gameBoard->map = {
        {-1, -1, -1, -1},
        {-1,  0,  1, -1},
        {-1,  1,  0, -1},
        {-1, -1, -1, -1}
    };
    gameBoard->rows = 4;
    gameBoard->cols = 4;

    // 测试激活第一个方块
    gameBoard->activateBlock(1, 1, 1);
    QVERIFY(gameBoard->isBlockActivated);
    QCOMPARE(gameBoard->lastActivatedBlock, QPair<int, int>(1, 1));

    // 测试激活匹配的第二个方块
    gameBoard->activateBlock(1, 2, 2);
    QVERIFY(!gameBoard->isBlockActivated);
    QCOMPARE(gameBoard->map[1][1], -1);
    QCOMPARE(gameBoard->map[2][2], -1);

    // 测试激活不匹配的第二个方块
    gameBoard->activateBlock(1, 1, 2);
    gameBoard->activateBlock(1, 2, 1);
    QVERIFY(gameBoard->isBlockActivated);
    QCOMPARE(gameBoard->lastActivatedBlock, QPair<int, int>(2, 1));
}

void TestGameBoard::testFindPath()
{
    // 设置一个已知的游戏状态
    gameBoard->map = {
        {-1, -1, -1, -1},
        {-1,  0, -1, -1},
        {-1, -1, -1,  0},
        {-1, -1, -1, -1}
    };
    gameBoard->rows = 4;
    gameBoard->cols = 4;

    // 测试可以连接的路径
    QVector<QPoint> path = gameBoard->findPath(1, 1, 2, 3);
    QVERIFY(!path.isEmpty());

    // 测试不可连接的路径
    path = gameBoard->findPath(1, 1, 3, 3);
    QVERIFY(path.isEmpty());
}

void TestGameBoard::testIsGameFinished()
{
    // 测试游戏未结束的情况
    gameBoard->map = {
        {-1, -1, -1},
        {-1,  0, -1},
        {-1, -1, -1}
    };
    gameBoard->rows = 3;
    gameBoard->cols = 3;
    QVERIFY(!gameBoard->isGameFinished());

    // 测试游戏结束的情况
    gameBoard->map = {
        {-1, -1, -1},
        {-1, -1, -1},
        {-1, -1, -1}
    };
    QVERIFY(gameBoard->isGameFinished());
}

void TestGameBoard::testHasMatchingPairs()
{
    // 测试有匹配对的情况
    gameBoard->map = {
        {-1,  0, -1},
        {-1,  1,  0},
        {-1, -1, -1}
    };
    gameBoard->rows = 3;
    gameBoard->cols = 3;
    QVERIFY(gameBoard->hasMatchingPairs());

    // 测试没有匹配对的情况
    gameBoard->map = {
        {-1,  0, -1},
        {-1,  1,  2},
        {-1, -1, -1}
    };
    QVERIFY(!gameBoard->hasMatchingPairs());
}

void TestGameBoard::testActivateBlockBoundary()
{
    // 测试边界位置的方块激活
    gameBoard->map = {
        { 0, -1, -1},
        {-1, -1,  0},
        {-1, -1, -1}
    };
    gameBoard->rows = 3;
    gameBoard->cols = 3;

    // 测试左上角方块
    gameBoard->activateBlock(1, 0, 0);
    QVERIFY(gameBoard->isBlockActivated);
    QCOMPARE(gameBoard->lastActivatedBlock, QPair<int, int>(0, 0));

    // 测试右下角方块
    gameBoard->activateBlock(1, 1, 2);
    QVERIFY(!gameBoard->isBlockActivated);
    QCOMPARE(gameBoard->map[0][0], -1);
    QCOMPARE(gameBoard->map[1][2], -1);

    // 测试无效位置
    gameBoard->activateBlock(1, -1, 0);
    QVERIFY(!gameBoard->isBlockActivated);
    gameBoard->activateBlock(1, 0, 3);
    QVERIFY(!gameBoard->isBlockActivated);
}

void TestGameBoard::testFindPathBoundary()
{
    gameBoard->map = {
        { 0, -1,  0},
        {-1, -1, -1},
        { 0, -1,  0}
    };
    gameBoard->rows = 3;
    gameBoard->cols = 3;

    // 测试边角到边角的路径
    QVector<QPoint> path = gameBoard->findPath(0, 0, 2, 2);
    QVERIFY(!path.isEmpty());

    // 测试边缘路径
    path = gameBoard->findPath(0, 0, 0, 2);
    QVERIFY(!path.isEmpty());

    // 测试无效路径
    path = gameBoard->findPath(-1, 0, 2, 2);
    QVERIFY(path.isEmpty());
    path = gameBoard->findPath(0, 0, 3, 3);
    QVERIFY(path.isEmpty());
}

void TestGameBoard::testGameBoardEdges()
{
    gameBoard->map = {
        { 0,  1,  2},
        { 3, -1,  4},
        { 5,  6,  0}
    };
    gameBoard->rows = 3;
    gameBoard->cols = 3;

    // 测试边缘方块的连接
    gameBoard->activateBlock(1, 0, 0);
    gameBoard->activateBlock(1, 2, 2);
    QCOMPARE(gameBoard->map[0][0], -1);
    QCOMPARE(gameBoard->map[2][2], -1);

    // 测试角落方块的连接
    gameBoard->map[0][0] = gameBoard->map[2][0] = 7;
    gameBoard->activateBlock(1, 0, 0);
    gameBoard->activateBlock(1, 2, 0);
    QCOMPARE(gameBoard->map[0][0], -1);
    QCOMPARE(gameBoard->map[2][0], -1);
}

void TestGameBoard::testLargeGameBoard()
{
    // 创建一个较大的游戏板
    gameBoard->rows = 20;
    gameBoard->cols = 20;
    gameBoard->map.resize(gameBoard->rows);
    for (int i = 0; i < gameBoard->rows; ++i) {
        gameBoard->map[i].resize(gameBoard->cols);
        for (int j = 0; j < gameBoard->cols; ++j) {
            gameBoard->map[i][j] = (i + j) % 3;  // 使用0, 1, 2循环填充
        }
    }

    // 测试大游戏板上的路径查找
    QVector<QPoint> path = gameBoard->findPath(0, 0, 19, 19);
    QVERIFY(!path.isEmpty());

    // 测试大游戏板上的方块激活
    gameBoard->activateBlock(1, 0, 0);
    gameBoard->activateBlock(1, 3, 3);  // 应该匹配
    QCOMPARE(gameBoard->map[0][0], -1);
    QCOMPARE(gameBoard->map[3][3], -1);
}

void TestGameBoard::testEmptyGameBoard()
{
    // 创建一个空的游戏板
    gameBoard->rows = 5;
    gameBoard->cols = 5;
    gameBoard->map = QVector<QVector<int>>(5, QVector<int>(5, -1));

    QVERIFY(gameBoard->isGameFinished());
    QVERIFY(!gameBoard->hasMatchingPairs());

    // 测试在空游戏板上激活方块
    gameBoard->activateBlock(1, 2, 2);
    QVERIFY(!gameBoard->isBlockActivated);
}

void TestGameBoard::testFullGameBoard()
{
    // 创建一个填满的游戏板，没有可匹配的对
    gameBoard->rows = 4;
    gameBoard->cols = 4;
    gameBoard->map = {
        {0, 1, 2, 3},
        {1, 2, 3, 0},
        {2, 3, 0, 1},
        {3, 0, 1, 2}
    };

    QVERIFY(!gameBoard->isGameFinished());
    QVERIFY(!gameBoard->hasMatchingPairs());

    // 测试在满游戏板上查找路径
    QVector<QPoint> path = gameBoard->findPath(0, 0, 3, 3);
    QVERIFY(path.isEmpty());
}

void TestGameBoard::testSingleBlockRemaining()
{
    // 创建一个只剩一个方块的游戏板
    gameBoard->rows = 3;
    gameBoard->cols = 3;
    gameBoard->map = {
        {-1, -1, -1},
        {-1,  0, -1},
        {-1, -1, -1}
    };

    QVERIFY(!gameBoard->isGameFinished());
    QVERIFY(!gameBoard->hasMatchingPairs());

    // 测试移除最后一个方块
    gameBoard->activateBlock(1, 1, 1);
    QVERIFY(gameBoard->isBlockActivated);
    gameBoard->map[1][1] = -1;  // 模拟移除方块
    QVERIFY(gameBoard->isGameFinished());
}
QTEST_MAIN(TestGameBoard)
#include "testgameboard.moc"class TestGameBoard : public QObject
