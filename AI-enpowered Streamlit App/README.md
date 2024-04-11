# Weather Forecast Streamlit App Powered by Vertex AI

## Setup

To install the necessary libraries, refer to the `requirements.txt` file.

Additionally, you will need a `key.json` file to connect to GCP/Vertex AI.

To run the app, enter the following command in your terminal:

 ```streamlit run main.py``` 


## App Components

Our weather app is deployed on Streamlit and powered by the OpenWeatherMap API and Vertex AI's Large Language Models (LLMs). Users can choose between two models: `text-bison@001`, which is a PaLM 2 text model, and `gemini pro 1.0`. The selected LLM is used to extract a city name from the user input and pass it to the weather API.

Once we have the information from the API, the app provides two output options: a simple summary of the weather and a detailed version with additional data for the user to read.

## Model Accuracy

Both models perform well with correct input. However, `text-bison@001` has a lower performance when switching between the detailed and simplified versions compared to `gemini pro 1.0`.

When the user input is incomplete (e.g., "pitts" for "Pittsburgh"), both models can usually complete the city name if the incomplete name is interpretable.

For incorrect input, `text-bison@001` has a lower performance than `gemini pro 1.0`. While `gemini pro 1.0` can often indicate that the input is incorrect, `text-bison@001` has a higher rate of hallucinations or adding random words to the output.

## Video Demonstration

https://cmu.box.com/s/35kvtb5eb1aliiu1rr1c8y1zerirmiu5
