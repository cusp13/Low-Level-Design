#include<iostream>
#include<string>

using namespace std;

class ModelTrainer {
public: 
    void trainPipeline(const string& dataPath) {
        loadData(dataPath);
        preprocessData();
        trainModel();
        evaluateModel();
        saveModel();
    }
    // these class are protected because no once can directly access this it should have some sequential steps.
protected: 
    void loadData(const string& path) {
        cout<<"[Common] Loading dataset form "<<path<<endl;
    }
    void preprocessData() {
        cout<<"[Common] data preprocessing started"<<endl;
    }
    virtual void trainModel() = 0;
    virtual void evaluateModel() = 0;
    virtual void saveModel(){
        cout<<"[Common] data model saved"<<endl;
    }
};

class NeuralNetwork : public ModelTrainer {
protected: 
    void trainModel() override {
        cout<<"[NeuralNetwork] Training Neural Network with capacity of 100 epochs"<<endl;
    }
    void evaluateModel() override {
        cout<<"[NeuralNetwork] Evaluating neural network"<<endl;
    }
    void saveModel() override {
        cout<<"[NeuralNet] Serializing network weights to .h5 file"<<endl;
    }
};

class DecisionTreeTrainer: public ModelTrainer {
protected: 
    void trainModel() override {
        cout << "[DecisionTree] Building decision tree with max_depth=5\n";
    }
    void evaluateModel() override {
        cout << "[DecisionTree] Computing classification report (precision/recall)\n";
    }
};

int main() {
    cout<<" === Neural Network Training === "<<endl;
    ModelTrainer* nnTrainer = new NeuralNetwork();
    nnTrainer->trainPipeline("data/images");
    cout<<"\n==Decision tree traning===\n";
    ModelTrainer* dtTrainer = new DecisionTreeTrainer();
    dtTrainer -> trainPipeline("data/iris.csv");

    return 0;
}