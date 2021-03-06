//
// @author raver119@gmail.com
//

#include "testlayers.h"
#include <flatbuffers/flatbuffers.h>
#include <graph/generated/node_generated.h>
#include <graph/generated/graph_generated.h>
#include <graph/Node.h>
#include <graph/Graph.h>
#include <NDArray.h>
#include <ops/declarable/declarable_ops.h>

using namespace nd4j::graph;

class GraphTests : public testing::Test {
public:
    int *cShape = new int[8]{2, 2, 2, 2, 1, 0, 1, 99};
    int *fShape = new int[8]{2, 2, 2, 1, 2, 0, 1, 102};
};

TEST_F(GraphTests, SingleInput1) {
    auto graph = new Graph<float>();

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    graph->getVariableSpace()->putVariable(-1, x);

    auto nodeA = new Node<float>(OpType_TRANSFORM, 0, 1, {-1}, {2});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 2, 2, {1}, {3});
    auto nodeC = new Node<float>(OpType_TRANSFORM, 0, 3, {2}, {});

    graph->addNode(nodeA);
    graph->addNode(nodeB);
    graph->addNode(nodeC);

    ASSERT_EQ(1, graph->rootNodes());
    ASSERT_EQ(3, graph->totalNodes());

    GraphExecutioner<float>::execute(graph);

    ASSERT_NEAR(0.4161468, x->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}

TEST_F(GraphTests, DoubleInput1) {
    auto graph = new Graph<float>();

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto y = new NDArray<float>(5, 5, 'c');
    y->assign(-1.0);

    auto z = new NDArray<float>(5, 5, 'c');

    graph->getVariableSpace()->putVariable(-1, x);
    graph->getVariableSpace()->putVariable(-2, y);
    graph->getVariableSpace()->putVariable(-3, z);

    auto nodeA = new Node<float>(OpType_TRANSFORM, 0, 1, {-1}, {3});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 0, 2, {-2}, {3});
    auto nodeC = new Node<float>(OpType_TRANSFORM, 0, 3, {1, 2}, {-3});

    graph->addNode(nodeA);
    graph->addNode(nodeB);
    graph->addNode(nodeC);

    ASSERT_EQ(2, graph->rootNodes());
    ASSERT_EQ(3, graph->totalNodes());

    GraphExecutioner<float>::execute(graph);

    ASSERT_NEAR(3.0, z->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}

TEST_F(GraphTests, SingleInput3) {
    auto graph = new Graph<float>();

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto v0 = new NDArray<float>(5, 5, 'c');
    auto v1 = new NDArray<float>(5, 5, 'c');

    graph->getVariableSpace()->putVariable(-1, x);
    graph->getVariableSpace()->putVariable(-2, v0);
    graph->getVariableSpace()->putVariable(-3, v1);

    auto nodeA = new Node<float>(OpType_TRANSFORM, 0, 1, {-1}, {2, 3});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 14, 2, {1}, {-2});
    auto nodeC = new Node<float>(OpType_TRANSFORM, 26, 3, {1}, {-3});

    graph->addNode(nodeA);
    graph->addNode(nodeB);
    graph->addNode(nodeC);

    ASSERT_EQ(1, graph->rootNodes());
    ASSERT_EQ(3, graph->totalNodes());

    GraphExecutioner<float>::execute(graph);

    ASSERT_NEAR(1.4142135, v0->reduceNumber<simdOps::Mean<float>>(), 1e-5);
    ASSERT_NEAR(1.0, v1->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}

TEST_F(GraphTests, SingleInput4) {
    auto graph = new Graph<float>();

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto v0 = new NDArray<float>(5, 5, 'c');
    auto v1 = new NDArray<float>(5, 5, 'c');

    graph->getVariableSpace()->putVariable(-1, x);
    graph->getVariableSpace()->putVariable(-2, v0);
    graph->getVariableSpace()->putVariable(-3, v1);

    auto nodeA = new Node<float>(OpType_TRANSFORM, 0, 1, {-1}, {2});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 14, 2, {1}, {3});
    auto nodeC = new Node<float>(OpType_TRANSFORM, 6, 3, {2}, {4, 5});

    auto nodeS = new Node<float>(OpType_TRANSFORM, 26, 4, {3}, {-2});
    auto nodeE = new Node<float>(OpType_TRANSFORM, 27, 5, {3}, {-3});

    graph->addNode(nodeA);
    graph->addNode(nodeB);
    graph->addNode(nodeC);
    graph->addNode(nodeS);
    graph->addNode(nodeE);

    ASSERT_EQ(1, graph->rootNodes());
    ASSERT_EQ(5, graph->totalNodes());

    GraphExecutioner<float>::execute(graph);

    ASSERT_NEAR(1.0, v0->reduceNumber<simdOps::Mean<float>>(), 1e-5);
    ASSERT_NEAR(-1.4142135, v1->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}


TEST_F(GraphTests, DoubleInput2) {
    auto graph = new Graph<float>();

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto y = new NDArray<float>(5, 5, 'c');
    y->assign(-1.0);

    auto z0 = new NDArray<float>(5, 5, 'c');
    auto z1 = new NDArray<float>(5, 5, 'c');

    graph->getVariableSpace()->putVariable(-1, x);
    graph->getVariableSpace()->putVariable(-2, y);
    graph->getVariableSpace()->putVariable(-3, z0);
    graph->getVariableSpace()->putVariable(-4, z1);


    auto nodeA = new Node<float>(OpType_TRANSFORM, 0, 1, {-1}, {2});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 14, 2, {1}, {3});
    auto nodeC = new Node<float>(OpType_TRANSFORM, 6, 3, {2}, {-3});

    auto nodeT = new Node<float>(OpType_TRANSFORM, 0, 11, {-2}, {12});
    auto nodeU = new Node<float>(OpType_TRANSFORM, 14, 12, {11}, {13});
    auto nodeV = new Node<float>(OpType_TRANSFORM, 6, 13, {12}, {-4});

    graph->addNode(nodeA);
    graph->addNode(nodeB);
    graph->addNode(nodeC);
    graph->addNode(nodeT);
    graph->addNode(nodeU);
    graph->addNode(nodeV);

    ASSERT_EQ(2, graph->rootNodes());
    ASSERT_EQ(6, graph->totalNodes());

    GraphExecutioner<float>::execute(graph);

    ASSERT_NEAR(-1.4142135, z0->reduceNumber<simdOps::Mean<float>>(), 1e-5);
    ASSERT_NEAR(-1.0, z1->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}


TEST_F(GraphTests, DoubleInput3) {
    auto graph = new Graph<float>();

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto y = new NDArray<float>(5, 5, 'c');
    y->assign(-1.0);

    auto z0 = new NDArray<float>(5, 5, 'c');
    auto z1 = new NDArray<float>(5, 5, 'c');


    auto w = new NDArray<float>(5, 5, 'c');

    graph->getVariableSpace()->putVariable(-1, x);
    graph->getVariableSpace()->putVariable(-2, y);
    graph->getVariableSpace()->putVariable(-3, z0);
    graph->getVariableSpace()->putVariable(-4, z1);
    graph->getVariableSpace()->putVariable(-5, w);


    auto nodeA = new Node<float>(OpType_TRANSFORM, 0, 1, {-1}, {2});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 14, 2, {1}, {3});
    auto nodeC = new Node<float>(OpType_TRANSFORM, 6, 3, {2}, {-3, 21});

    auto nodeT = new Node<float>(OpType_TRANSFORM, 0, 11, {-2}, {12});
    auto nodeU = new Node<float>(OpType_TRANSFORM, 14, 12, {11}, {13});
    auto nodeV = new Node<float>(OpType_TRANSFORM, 6, 13, {12}, {-4, 21});

    auto nodeW = new Node<float>(OpType_TRANSFORM, 0, 21, {3, 13}, {22});
    auto nodeZ = new Node<float>(OpType_TRANSFORM, 0, 22, {21}, {-5});

    graph->addNode(nodeA);
    graph->addNode(nodeB);
    graph->addNode(nodeC);
    graph->addNode(nodeT);
    graph->addNode(nodeU);
    graph->addNode(nodeV);
    graph->addNode(nodeW);
    graph->addNode(nodeZ);

    ASSERT_EQ(2, graph->rootNodes());
    ASSERT_EQ(8, graph->totalNodes());

    GraphExecutioner<float>::execute(graph);

    ASSERT_NEAR(-1.4142135, z0->reduceNumber<simdOps::Mean<float>>(), 1e-5);
    ASSERT_NEAR(-1.0, z1->reduceNumber<simdOps::Mean<float>>(), 1e-5);

    ASSERT_NEAR(2.4142135, w->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}


TEST_F(GraphTests, QuadInput1) {
    auto graph = new Graph<float>();

    auto x0 = new NDArray<float>(5, 5, 'c');
    x0->assign(0.0);

    auto x1 = new NDArray<float>(5, 5, 'c');
    x1->assign(-1.0);

    auto x2 = new NDArray<float>(5, 5, 'c');
    x2->assign(-2.0);

    auto x3 = new NDArray<float>(5, 5, 'c');
    x3->assign(-3.0);

    auto z = new NDArray<float>(5, 5, 'c');
    z->assign(119.0);

    graph->getVariableSpace()->putVariable(-1, x0);
    graph->getVariableSpace()->putVariable(-2, x1);
    graph->getVariableSpace()->putVariable(-3, x2);
    graph->getVariableSpace()->putVariable(-4, x3);
    graph->getVariableSpace()->putVariable(-5, z);

    auto nodeA = new Node<float>(OpType_TRANSFORM, 0, 1, {-1}, {11});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 0, 2, {-2}, {11});
    auto nodeC = new Node<float>(OpType_TRANSFORM, 0, 3, {-3}, {21});
    auto nodeD = new Node<float>(OpType_TRANSFORM, 0, 4, {-4}, {21});

    auto nodeP1 = new Node<float>(OpType_TRANSFORM, 0, 11, {1, 2}, {31});
    auto nodeP2 = new Node<float>(OpType_TRANSFORM, 0, 21, {3, 4}, {31});

    auto nodeZ = new Node<float>(OpType_TRANSFORM, 0, 31, {11, 21}, {-5});

    graph->addNode(nodeA);
    graph->addNode(nodeB);
    graph->addNode(nodeC);
    graph->addNode(nodeD);
    graph->addNode(nodeP1);
    graph->addNode(nodeP2);
    graph->addNode(nodeZ);

    ASSERT_EQ(4, graph->rootNodes());
    ASSERT_EQ(7, graph->totalNodes());

    GraphExecutioner<float>::execute(graph);

    ASSERT_NEAR(6.0, z->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}

TEST_F(GraphTests, InternalBranching1) {
    auto graph = new Graph<float>();

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(0.0);

    auto z = new NDArray<float>(5, 5, 'c');

    graph->getVariableSpace()->putVariable(-1, x);
    graph->getVariableSpace()->putVariable(-2, z);

    // 1.0
    auto nodeA = new Node<float>(OpType_TRANSFORM, 26, 1, {-1}, {11, 21});

    // -1
    auto nodeK = new Node<float>(OpType_TRANSFORM, 6, 11, {1}, {12});

    // 2.0
    auto nodeL = new Node<float>(OpType_TRANSFORM, 35, 12, {11}, {31});

    // -1
    auto nodeR = new Node<float>(OpType_TRANSFORM, 6, 21, {1}, {22});

    // 1
    auto nodeS = new Node<float>(OpType_TRANSFORM, 6, 22, {21}, {31});

    // 1.0
    auto nodeZ = new Node<float>(OpType_TRANSFORM, 0, 31, {12, 22}, {-2});

    graph->addNode(nodeA);
    graph->addNode(nodeK);
    graph->addNode(nodeL);
    graph->addNode(nodeR);
    graph->addNode(nodeS);
    graph->addNode(nodeZ);

    ASSERT_EQ(1, graph->rootNodes());
    ASSERT_EQ(6, graph->totalNodes());

    GraphExecutioner<float>::execute(graph);

    ASSERT_EQ(3, nodeZ->getLayer());

    ASSERT_NEAR(3.0, z->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}


TEST_F(GraphTests, ReductionsTest1) {
    auto graph = new Graph<float>();

    auto x = new NDArray<float>(5, 5, 'c');
    for (int r = 0; r < x->rows(); r++) {
        for (int c = 0; c < x->columns(); c++) {
            x->putScalar(r, c, -c);
        }
    }

    auto z = new NDArray<float>(1, 5, 'c');

    graph->getVariableSpace()->putVariable(-1, x);
    graph->getVariableSpace()->putVariable(-2, z);


    auto nodeA = new Node<float>(OpType_ACCUMULATION, 0, 1, {-1}, {2}, {1});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 0, 2, {1}, {-2});

    graph->addNode(nodeA);
    graph->addNode(nodeB);

    ASSERT_EQ(1, graph->rootNodes());
    ASSERT_EQ(2, graph->totalNodes());

    GraphExecutioner<float>::execute(graph);

    ASSERT_NEAR(2.0, z->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}


TEST_F(GraphTests, IndexReductionsTest1) {
    auto graph = new Graph<float>();

    auto x = new NDArray<float>(5, 5, 'c');
    for (int r = 0; r < x->rows(); r++) {
        for (int c = 0; c < x->columns(); c++) {
            x->putScalar(r, c, -c);
        }
    }

    auto z = new NDArray<float>(1, 5, 'c');

    graph->getVariableSpace()->putVariable(-1, x);
    graph->getVariableSpace()->putVariable(-2, z);


    auto nodeA = new Node<float>(OpType_INDEX_ACCUMULATION, 1, 1, {-1}, {2}, {1});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 0, 2, {1}, {-2});

    graph->addNode(nodeA);
    graph->addNode(nodeB);

    ASSERT_EQ(1, graph->rootNodes());
    ASSERT_EQ(2, graph->totalNodes());

    GraphExecutioner<float>::execute(graph);

    ASSERT_NEAR(4.0, z->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}


TEST_F(GraphTests, AutoOutput1) {
    auto graph = new Graph<float>();
    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    graph->getVariableSpace()->putVariable(-1, x);

    auto nodeA = new Node<float>(OpType_TRANSFORM, 0, 1, {-1}, {2});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 35, 2, {1}, {});

    graph->addNode(nodeA);
    graph->addNode(nodeB);

    ASSERT_EQ(1, graph->rootNodes());
    ASSERT_EQ(2, graph->totalNodes());

    graph->buildGraph();

    ASSERT_TRUE(graph->getVariableSpace()->getVariable(2) != nullptr);

    GraphExecutioner<float>::execute(graph);

    auto outputs = graph->fetchOutputs();

    ASSERT_EQ(1, outputs->size());

    ASSERT_TRUE(outputs->at(0) != nullptr);

    ASSERT_NEAR(-1.0, outputs->at(0)->getNDArray()->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}


TEST_F(GraphTests, AutoOutput2) {
    auto graph = new Graph<float>();
    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    graph->getVariableSpace()->putVariable(-1, x);

    auto nodeA = new Node<float>(OpType_TRANSFORM, 0, 1, {-1}, {2, 3, -1});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 35, 2, {1}, {});
    auto nodeC = new Node<float>(OpType_TRANSFORM, 6, 3, {1}, {});

    graph->addNode(nodeA);
    graph->addNode(nodeB);
    graph->addNode(nodeC);

    ASSERT_EQ(1, graph->rootNodes());
    ASSERT_EQ(3, graph->totalNodes());

    graph->buildGraph();

    ASSERT_TRUE(graph->getVariableSpace()->getVariable(-1) != nullptr);
    ASSERT_TRUE(graph->getVariableSpace()->getVariable(2) != nullptr);
    ASSERT_TRUE(graph->getVariableSpace()->getVariable(3) != nullptr);

    GraphExecutioner<float>::execute(graph);

    auto outputs = graph->fetchOutputs();

    ASSERT_EQ(2, outputs->size());

    ASSERT_TRUE(outputs->at(0) != nullptr);

    ASSERT_NEAR(-1.0, outputs->at(0)->getNDArray()->reduceNumber<simdOps::Mean<float>>(), 1e-5);
    ASSERT_NEAR(-2.0, outputs->at(1)->getNDArray()->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}


TEST_F(GraphTests, BroadcastTest1) {
    auto graph = new Graph<float>();
    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(0.0);

    auto y = new NDArray<float>(1, 5, 'c');
    for (int e = 0; e < y->columns(); e++) {
        y->putScalar(e, e);
    }

    auto z = new NDArray<float>(5, 5, 'c');

    graph->getVariableSpace()->putVariable(-1, x);
    graph->getVariableSpace()->putVariable(-2, y);
    graph->getVariableSpace()->putVariable(-3, z);

    auto nodeA = new Node<float>(OpType_BROADCAST, 0, 1, {-1, -2}, {2}, {1});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 6, 2, {1}, {-3});

    graph->addNode(nodeA);
    graph->addNode(nodeB);

    GraphExecutioner<float>::execute(graph);

    ASSERT_NEAR(-2.0, z->reduceNumber<simdOps::Mean<float>>(), 1e-5);

}


TEST_F(GraphTests, ScalarTest1) {
    auto graph = new Graph<float>();

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto z = new NDArray<float>(5, 5, 'c');

    graph->getVariableSpace()->putVariable(-1, x);
    graph->getVariableSpace()->putVariable(-2, z);

    auto nodeA = new Node<float>(OpType_TRANSFORM, 0, 1, {-1}, {2});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 14, 2, {1}, {3});
    auto nodeE = new Node<float>(OpType_SCALAR, 0, 3, {2}, {-2}, {}, 1.3f);

    graph->addNode(nodeA);
    graph->addNode(nodeB);
    graph->addNode(nodeE);

    ASSERT_EQ(1, graph->rootNodes());
    ASSERT_EQ(3, graph->totalNodes());

    GraphExecutioner<float>::execute(graph);

    ASSERT_NEAR(2.714213, z->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}

TEST_F(GraphTests, SymbolicLookupTest1) {
    auto graph = new Graph<float>();

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto z = new NDArray<float>(5, 5, 'c');

    Variable<float> vX(x);
    Variable<float> vZ(z);

    vX.setName(new std::string("alpha"));
    vZ.setName(new std::string("omega"));

    graph->getVariableSpace()->putVariable(-1, &vX);
    graph->getVariableSpace()->putVariable(-2, &vZ);

    auto nodeA = new Node<float>(OpType_TRANSFORM, 0, 1, {-1}, {2});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 14, 2, {1}, {-2});

    nodeA->setName(new std::string("phi"));
    nodeB->setName(new std::string("theta"));

    graph->addNode(nodeA);
    graph->addNode(nodeB);


    auto rX = graph->getVariableSpace()->getVariable(new std::string("alpha"));
    auto rZ = graph->getVariableSpace()->getVariable(new std::string("omega"));

    ASSERT_TRUE(rX->getNDArray() == vX.getNDArray());
    ASSERT_TRUE(rZ->getNDArray() == vZ.getNDArray());
    ASSERT_FALSE(graph->getVariableSpace()->hasVariable(new std::string("omicron")));


    ASSERT_TRUE(graph->getVariableSpace()->hasVariable(1));
    ASSERT_TRUE(graph->getVariableSpace()->hasVariable(2));

    GraphExecutioner<float>::execute(graph);

    ASSERT_TRUE(graph->getVariableSpace()->hasVariable(new std::string("phi")));
    ASSERT_TRUE(graph->getVariableSpace()->hasVariable(new std::string("theta")));

    ASSERT_NEAR(1.4142135, z->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}

TEST_F(GraphTests, OutputValidation1) {
    auto graph = new Graph<float>();

    graph->getExecutorConfiguration()->_outputMode = OutputMode_EXPLICIT;

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto z = new NDArray<float>(5, 5, 'c');

    Variable<float> vX(x);
    Variable<float> vZ(z);

    vX.setName(new std::string("alpha"));
    vZ.setName(new std::string("omega"));

    graph->getVariableSpace()->putVariable(-1, &vX);
    graph->getVariableSpace()->putVariable(-2, &vZ);

    auto nodeA = new Node<float>(OpType_TRANSFORM, 0, 1, {-1}, {2});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 14, 2, {1}, {-2});

    graph->addNode(nodeA);
    graph->addNode(nodeB);

    ASSERT_EQ(0, graph->fetchOutputs()->size());
}

TEST_F(GraphTests, OutputValidation2) {
    auto graph = new Graph<float>();

    graph->getExecutorConfiguration()->_outputMode = OutputMode_EXPLICIT;

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto z = new NDArray<float>(5, 5, 'c');

    Variable<float> vX(x);
    Variable<float> vZ(z);

    vX.setName(new std::string("alpha"));
    vZ.setName(new std::string("omega"));

    graph->getVariableSpace()->putVariable(-1, &vX);
    graph->getVariableSpace()->putVariable(-2, &vZ);

    auto nodeA = new Node<float>(OpType_TRANSFORM, 0, 1, {-1}, {2});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 14, 2, {1}, {-2});

    graph->addNode(nodeA);
    graph->addNode(nodeB);

    graph->addOutput(-2);

    GraphExecutioner<float>::execute(graph);

    ASSERT_EQ(1, graph->fetchOutputs()->size());

    ASSERT_NEAR(1.4142135, graph->fetchOutputs()->at(0)->getNDArray()->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}

TEST_F(GraphTests, OutputValidation3) {
    auto graph = new Graph<float>();

    graph->getExecutorConfiguration()->_outputMode = OutputMode_IMPLICIT;

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto z = new NDArray<float>(5, 5, 'c');

    Variable<float> vX(x);
    Variable<float> vZ(z);

    vX.setName(new std::string("alpha"));
    vZ.setName(new std::string("omega"));

    graph->getVariableSpace()->putVariable(-1, &vX);
    graph->getVariableSpace()->putVariable(-2, &vZ);

    auto nodeA = new Node<float>(OpType_TRANSFORM, 0, 1, {-1}, {2});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 14, 2, {1}, {-2});

    graph->addNode(nodeA);
    graph->addNode(nodeB);

    GraphExecutioner<float>::execute(graph);

    ASSERT_EQ(1, graph->fetchOutputs()->size());

    ASSERT_NEAR(1.4142135, graph->fetchOutputs()->at(0)->getNDArray()->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}

TEST_F(GraphTests, OutputValidation4) {
    auto graph = new Graph<float>();

    graph->getExecutorConfiguration()->_outputMode = OutputMode_EXPLICIT_AND_IMPLICIT;

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto z = new NDArray<float>(5, 5, 'c');

    Variable<float> vX(x);
    Variable<float> vZ(z);

    vX.setName(new std::string("alpha"));
    vZ.setName(new std::string("omega"));

    graph->getVariableSpace()->putVariable(-1, &vX);
    graph->getVariableSpace()->putVariable(-2, &vZ);

    auto nodeA = new Node<float>(OpType_TRANSFORM, 0, 1, {-1}, {2});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 14, 2, {1}, {-2});

    graph->addOutput(-1);

    // not a typo. we want this value only once
    graph->addOutput(-1);

    graph->addNode(nodeA);
    graph->addNode(nodeB);

    GraphExecutioner<float>::execute(graph);

    ASSERT_EQ(2, graph->fetchOutputs()->size());

    ASSERT_NEAR(1.4142135, graph->fetchOutputs()->at(1)->getNDArray()->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}


TEST_F(GraphTests, OutputValidation5) {
    auto graph = new Graph<float>();

    graph->getExecutorConfiguration()->_outputMode = OutputMode_VARIABLE_SPACE;

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto z = new NDArray<float>(5, 5, 'c');

    Variable<float> vX(x);
    Variable<float> vZ(z);

    vX.setName(new std::string("alpha"));
    vZ.setName(new std::string("omega"));

    graph->getVariableSpace()->putVariable(-1, &vX);
    graph->getVariableSpace()->putVariable(-2, &vZ);

    auto nodeA = new Node<float>(OpType_TRANSFORM, 0, 1, {-1}, {2});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 14, 2, {1}, {-2});

    graph->addOutput(-1);

    graph->addNode(nodeA);
    graph->addNode(nodeB);

    GraphExecutioner<float>::execute(graph);

    ASSERT_EQ(3, graph->fetchOutputs()->size());

    //ASSERT_NEAR(1.4142135, graph->fetchOutputs()->at(1)->getNDArray()->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}

TEST_F(GraphTests, OutputValidation6) {
    auto graph = new Graph<float>();

    graph->getExecutorConfiguration()->_outputMode = OutputMode_VARIABLE_SPACE;

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto z = new NDArray<float>(5, 5, 'c');

    Variable<float> vX(x);
    Variable<float> vZ(z);

    vX.setName(new std::string("alpha"));
    vZ.setName(new std::string("omega"));

    graph->getVariableSpace()->putVariable(-1, &vX);
    graph->getVariableSpace()->putVariable(-2, &vZ);

    auto nodeA = new Node<float>(OpType_TRANSFORM, 0, 1, {-1}, {2});
    auto nodeB = new Node<float>(OpType_TRANSFORM, 14, 2, {1}, {});

    //graph->addOutput(-1);

    graph->addNode(nodeA);
    graph->addNode(nodeB);

    GraphExecutioner<float>::execute(graph);


    nd4j_printf("Returned variables: \n", "");
    for (int e = 0; e < graph->fetchOutputs()->size(); e++) {
        printf("%i, ", graph->fetchOutputs()->at(e)->id());
    }
    printf("\n");

    ASSERT_EQ(4, graph->fetchOutputs()->size());

    //ASSERT_NEAR(1.4142135, graph->fetchOutputs()->at(1)->getNDArray()->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}

TEST_F(GraphTests, TestMultiOutput1) {
    auto graph = new Graph<float>();

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto y = new NDArray<float>(5, 5, 'c');
    y->assign(-3.0);

    graph->getVariableSpace()->putVariable(-1, x);
    graph->getVariableSpace()->putVariable(-2, y);


    // Abs
    auto nodeA0 = new Node<float>(OpType_TRANSFORM, 0, 1, {-1}, {11});
    auto nodeB0 = new Node<float>(OpType_TRANSFORM, 0, 2, {-2}, {11});

    auto op = nd4j::ops::OpRegistrator::getInstance()->getOperationFloat("TestOp2i2o");

    // this op will add 1.0 to first input, and 2.0 for second input
    auto nodeT = new Node<float>(OpType_CUSTOM, 0, 11, {1, 2}, {21, 31}, {}, 0.0f);
    nodeT->setName("TestOp2i2o");
    nodeT->setCustomOp(op);


    // this op will subtract this value from 1.0
    auto nodeX = new Node<float>(OpType_TRANSFORM, 35, 21);
    nodeX->pickInput(11, 0);

    // this op will subtract this value from 1.0
    auto nodeY = new Node<float>(OpType_TRANSFORM, 35, 31);
    nodeY->pickInput(11, 1);

    graph->addNode(nodeA0);
    graph->addNode(nodeB0);
    graph->addNode(nodeT);
    graph->addNode(nodeX);
    graph->addNode(nodeY);

    std::pair<int, int> pair0(11,0);
    std::pair<int, int> pair1(11,1);

    ASSERT_TRUE(graph->getVariableSpace()->hasVariable(pair0));
    ASSERT_TRUE(graph->getVariableSpace()->hasVariable(pair1));

    GraphExecutioner<float>::execute(graph);

    ASSERT_NEAR(-2.0f, graph->getVariableSpace()->getVariable(21)->getNDArray()->meanNumber(), 1e-5);
    ASSERT_NEAR(-4.0f, graph->getVariableSpace()->getVariable(31)->getNDArray()->meanNumber(), 1e-5);
}

TEST_F(GraphTests, TestDivergentNode1) {
    auto op = nd4j::ops::OpRegistrator::getInstance()->getOperationFloat("Switch");
    auto nodeY = new Node<float>(OpType_CUSTOM, 0, 1);
    nodeY->setCustomOp(op);

    ASSERT_TRUE(nodeY->isDivergencePoint());
    ASSERT_TRUE(nodeY->isActive());
}