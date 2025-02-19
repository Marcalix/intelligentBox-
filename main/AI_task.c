#include "AI_task.h"

#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_profiler.h"
#include "tensorflow/lite/micro/recording_micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

// Importer le modèle converti en tableau C
#include "model.cc"

// Configuration mémoire
constexpr int kTensorArenaSize = 128 * 1024;  // Ajuster selon la mémoire disponible
alignas(16) uint8_t tensor_arena[kTensorArenaSize];

// Déclaration des objets globaux
static tflite::MicroErrorReporter micro_error_reporter;
static tflite::AllOpsResolver resolver;
static tflite::MicroInterpreter* interpreter = nullptr;
tflite::ErrorReporter* error_reporter = &micro_error_reporter;
const tflite::Model* model = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

// Fonction pour initialiser le modèle
void init_model() {
    model = tflite::GetModel(model_data);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        printf("Modèle incompatible avec la version de TensorFlow Lite Micro !\n");
        return;
    }

    static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
    interpreter = &static_interpreter;
    interpreter->AllocateTensors();

    input = interpreter->input(0);
    output = interpreter->output(0);
}

// Fonction pour charger les données d'entrée
void load_input_data(float* data, int length) {
    if (input == nullptr) return;
    for (int i = 0; i < length && i < input->dims->data[1]; i++) {
        input->data.f[i] = data[i];
    }
}

// Fonction pour exécuter l'inférence
void run_inference() {
    if (interpreter == nullptr) return;
    if (interpreter->Invoke() != kTfLiteOk) {
        printf("Échec de l'inférence\n");
    }
}

// Fonction pour afficher le résultat
void print_result() {
    if (output == nullptr) return;
    printf("Résultat : %f\n", output->data.f[0]);  // Modifier selon la sortie du modèle
}

void app_main() {
    init_model();
    
    float sample_data[10] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0}; // Exemple de données
    load_input_data(sample_data, 10);
    
    run_inference();
    print_result();
}