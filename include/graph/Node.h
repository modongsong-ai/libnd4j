//
// @author raver119@gmail.com
//

#ifndef LIBND4J_GNODE_H
#define LIBND4J_GNODE_H

#include <atomic>
#include <string>
#include <NDArray.h>
#include <ops/declarable/declarable_ops.h>
#include <graph/generated/node_generated.h>


namespace nd4j {
    namespace graph {

        template <typename T>
        class Node {
        protected:
            DataType _dataType;
            OpType _opType;
            Block<T>* _block;
            int _opNum;
            int _id;
            std::vector<std::pair<int, int>> _input;
            std::vector<int> _output;
            std::vector<int> _dimensions;

            int * _dim;
            std::string _name;


            // this variable points to onion layer within graph
            int _layer = -1;

            // many ops require extra parameters to run
            T *_extraParams;

            // optional scalar. used in scalar ops and in summary stats
            float _scalar;

            bool _hasExternalOutputs;
            bool _hasExternalInputs;
            bool _hasInternalOutputs;
            bool _hasInternalInputs;

            nd4j::ops::DeclarableOp<T> *_customOp = nullptr;

            bool _active = true;

        public:
            Node(OpType opType = OpType_TRANSFORM, int opNum = 0, int id = 0, std::initializer_list<int> input = {}, std::initializer_list<int> output = {},  std::initializer_list<int> dimensions = {}, float scalar = 0.0f);
            Node(const nd4j::graph::FlatNode *node);
            ~Node();

            bool equals(Node *other);

            OpType opType();
            int opNum();
            int id();
            std::vector<std::pair<int,int>> *input();
            std::vector<int> *output();

            void setId(int id);

            T *extraParams();

            bool isMultiInput();
            bool isMultiOutput();

            int getLayer();
            void setLayer(int layer);

            bool isDivergencePoint();
            void setActive(bool reallyActive);
            bool isActive();

            bool hasExternalOutputs();
            bool hasExternalInputs();
            bool hasInternalOutputs();
            bool hasInternalInputs();

            T scalar();

            std::vector<int> * getDimensions();
            int * getDimensionsPtr();


            void pickOutput(int outputId);
            void pickExternalOutput(int outputId);
            void pickInput(int inputId);
            void pickInput(int nodeId, int outputId);
            void pickInput(std::pair<int,int>& id);

            void setName(std::string *name);
            void setName(const std::string& name);
            std::string * getName();


            void setBlock(Block<T> *block);
            Block<T>* getBlock();

            void setCustomOp(nd4j::ops::DeclarableOp<T> *customOp = nullptr);
            nd4j::ops::DeclarableOp<T>* getCustomOp();
            bool hasCustomOp();
        };
    }
}

template <typename T>
bool nd4j::graph::Node<T>::isDivergencePoint() {
    if (hasCustomOp()) {
        return _customOp->getOpDescriptor()->isDivergent();
    } else
        return false;
}

template <typename T>
void nd4j::graph::Node<T>::setActive(bool reallyActive) {
    _active = reallyActive;
}

template <typename T>
bool nd4j::graph::Node<T>::isActive() {
    return _active;
}

template <typename T>
Block<T> * nd4j::graph::Node<T>::getBlock() {
    return _block;
}

template <typename T>
void nd4j::graph::Node<T>::setBlock(Block<T> *block) {
    _block = block;
}

template <typename T>
void nd4j::graph::Node<T>::setId(int id) {
    _id = id;
}

template <typename T>
nd4j::ops::DeclarableOp<T>* nd4j::graph::Node<T>::getCustomOp() {
    return _customOp;
}

template <typename T>
void nd4j::graph::Node<T>::setCustomOp(nd4j::ops::DeclarableOp<T> *customOp) {
    _customOp = customOp;
}

template <typename T>
bool nd4j::graph::Node<T>::hasCustomOp() {
    return _customOp != nullptr;
}

template <typename T>
std::string * nd4j::graph::Node<T>::getName() {
    return &_name;
}

template <typename T>
void nd4j::graph::Node<T>::setName(const std::string& name) {
    _name = name.c_str();
}

template <typename T>
void nd4j::graph::Node<T>::setName(std::string *name) {
    _name = *name;
}

template <typename T>
T nd4j::graph::Node<T>::scalar() {
    return (T) _scalar;
};

template <typename T>
void nd4j::graph::Node<T>::pickInput(std::pair<int,int>& pair) {
    _input.push_back(pair);
}

template <typename T>
void nd4j::graph::Node<T>::pickInput(int inputId, int outputId) {
    std::pair<int,int> p(inputId,outputId);
    pickInput(p);
}

template <typename T>
void nd4j::graph::Node<T>::pickInput(int inputId) {
    pickInput(inputId, 0);

    if (inputId < 0)
        _hasExternalInputs = true;
    else
        _hasInternalInputs = true;
}

template <typename T>
void nd4j::graph::Node<T>::pickExternalOutput(int outputId) {
    _output.push_back(outputId);

    _hasExternalOutputs = true;
}

template <typename T>
void nd4j::graph::Node<T>::pickOutput(int outputId) {
    _output.push_back(outputId);

    if (outputId < 0)
        _hasExternalOutputs = true;
    else
        _hasInternalOutputs = true;
}

template <typename T>
int * nd4j::graph::Node<T>::getDimensionsPtr() {
    return _dim;
}

template <typename T>
std::vector<int> * nd4j::graph::Node<T>::getDimensions() {
    return &_dimensions;
}

template <typename T>
int nd4j::graph::Node<T>::getLayer() {
    return _layer;
}

template <typename T>
void nd4j::graph::Node<T>::setLayer(int layer) {
    _layer = layer;
}

template <typename T>
bool nd4j::graph::Node<T>::hasExternalOutputs() {
    return _hasExternalOutputs;
}

template <typename T>
bool nd4j::graph::Node<T>::hasExternalInputs() {
    return _hasExternalInputs;
}

template <typename T>
bool nd4j::graph::Node<T>::hasInternalOutputs() {
    return _hasInternalOutputs;
}

template <typename T>
bool nd4j::graph::Node<T>::hasInternalInputs() {
    return _hasInternalInputs;
}

template <typename T>
bool nd4j::graph::Node<T>::isMultiInput() {
    return _input.size() > 1;
}

template <typename T>
bool nd4j::graph::Node<T>::isMultiOutput() {
    return _output.size() > 1;
}

template <typename T>
T * nd4j::graph::Node<T>::extraParams() {
    return _extraParams;
}

template <typename T>
nd4j::graph::OpType nd4j::graph::Node<T>::opType() {
    return _opType;
}

template <typename T>
int nd4j::graph::Node<T>::id() {
    return _id;
}

template <typename T>
int nd4j::graph::Node<T>::opNum() {
    return _opNum;
}

template <typename T>
std::vector<std::pair<int,int>> *nd4j::graph::Node<T>::input() {
    return &_input;
}

template <typename T>
std::vector<int> *nd4j::graph::Node<T>::output() {
    return &_output;
}

template <typename T>
nd4j::graph::Node<T>::Node(OpType opType, int opNum, int id, std::initializer_list<int> input, std::initializer_list<int> output, std::initializer_list<int> dimensions, float scalar) {
    this->_opType = opType;
    this->_id = id;
    this->_opNum = opNum;

    _hasExternalInputs = false;
    _hasExternalOutputs = false;
    _hasInternalInputs = false;
    _hasInternalOutputs = false;

    _scalar = scalar;

    for (auto i: input)
        pickInput(i);

    for (auto o: output)
        pickOutput(o);


    if (dimensions.size() > 0) {
        _dim = new int[dimensions.size()];
        int cnt = 0;
        for (auto d: dimensions) {
            _dimensions.push_back(d);
            _dim[cnt++] = d;
        }
    }

};

template <typename T>
nd4j::graph::Node<T>::Node(const nd4j::graph::FlatNode *node) {
    _hasExternalInputs = false;
    _hasExternalOutputs = false;
    _hasInternalInputs = false;
    _hasInternalOutputs = false;

    _scalar = node->scalar();

    if (node != nullptr) {
        this->_id = node->id();
        this->_dataType = node->dataType();
        this->_opNum = node->opNum();
        this->_opType = node->opType();

        if (node->name() != nullptr && node->name()->c_str() != nullptr)
            this->_name = node->name()->str();

        nd4j_printf("Pulled node_%i (%s)\n", node->id(), this->_name.c_str())

        if (node->input() != nullptr)
            for (int e = 0; e < (int) node->input()->size(); e++)
                pickInput(node->input()->Get(e));

        if (node->output() != nullptr)
            for (int e = 0; e < (int) node->output()->size(); e++) {
                nd4j_verbose("Picking output: %i\n", node->output()->Get(e));
                pickOutput(node->output()->Get(e));
            }


        if (node->extraParams() != nullptr && node->extraParams()->size() > 0) {
            _extraParams = new T[node->extraParams()->size()];
            for (int e = 0; e < (int) node->extraParams()->size(); e++) {
                _extraParams[e] = (T) node->extraParams()->Get(e);
            }
        }

        if (node->dimensions() != nullptr && node->dimensions()->size() > 0) {
            _dim = new int[node->dimensions()->size()];
            for (int e = 0; e < (int) node->dimensions()->size(); e++) {
                _dimensions.push_back(node->dimensions()->Get(e));
                _dim[e] = node->dimensions()->Get(e);
            }
        }
    } else {
        // empty dynamic node, tests probably
    }
}

template <typename T>
nd4j::graph::Node<T>::~Node() {
    if (_extraParams != nullptr)
        delete[] _extraParams;

    if (_dim != nullptr)
        delete[] _dim;
}

template <typename T>
bool nd4j::graph::Node<T>::equals(Node *other) {
    if (_opType == other->_opType && _dataType == other->_dataType && _opNum == other->_opNum)
        return true;

    return false;
}


#endif //LIBND4J_GNODE_H
