import streamlit as st
import os
import vertexai
from langchain_google_vertexai import VertexAI
from langchain import PromptTemplate
from langchain.chains import LLMChain, SequentialChain
from langchain_community.utilities.openweathermap import OpenWeatherMapAPIWrapper
from langchain.agents import AgentType, initialize_agent, load_tools

# Load necessary environment variables
os.environ["GOOGLE_APPLICATION_CREDENTIALS"]="./key.json" # place the key JSON file in the same folder as your notebook
os.environ["OPENWEATHERMAP_API_KEY"] = '873b5e79e3cf4b90a37d36fa352284bd'
PROJECT_ID = "gen-ai-412319"
REGION = "us-central1"
BUCKET_URI = f"gs://gen-ai-buckets"

# Initialize Vertex AI
try:
    vertexai.init(project=PROJECT_ID, location=REGION, staging_bucket=BUCKET_URI)
except Exception as e:
    print(f"Encountered error while initializing Vertex AI. Error code: {e}")
    # Terminate the program
    exit()

# Initialize LLMs
try:
    llm_bison = VertexAI(
        model_name="text-bison@001",
        max_output_tokens=256,
        temperature=0.2,
        top_p=0.8,
        top_k=40,
        verbose=True,
    )
except Exception as e:
    print(f"Encountered error while initializing LLM Bison. Error code: {e}")
    # Terminate the program
    exit()

llm_gemini = VertexAI(
    model_name="gemini-1.0-pro-vision",
    max_output_tokens=256,
    temperature=0.5,
    verbose=True,
)

# Web App components
st.title("City Weather Report")


input = st.text_input('What city would you like to get the weather for?')
methods = st.sidebar.selectbox("Select a model to use", ("text-bison", "gemini-pro"))
on = st.toggle('Detailed Weather Report')
col1, col2, col3 = st.sidebar.columns(3)
generate_result = col2.button("Ready to Answer!")

if generate_result:
    # User choose which model to use
    if methods == "text-bison":
        llm = llm_bison
    else:
        llm = llm_gemini

    # Extract city name from input
    extract_place_template = PromptTemplate(
        input_variables=['input'],
        template="The following input contains a city name, return that city name only, correct the spelling of the city name if necessary. If you are not able to pinpoint a name from the input or the given name is not a valid city name then output the word false in lower case: {input}."
    )
    extract_chain = LLMChain(llm = llm, prompt = extract_place_template, output_key='name')
    chain = SequentialChain(input_variables=['input'], chains=[extract_chain], output_variables=['name'])
    result = chain.run(input=input)
    st.write(f'Your inputted city name is: {result}')

    # Fetch weather data
    if 'false' in result.lower():
        st.write("Please enter a valid city name and try again.")
    else:
        tools = load_tools(["openweathermap-api"], llm)
        agent_chain = initialize_agent(
            tools=tools, llm=llm, agent=AgentType.ZERO_SHOT_REACT_DESCRIPTION, verbose=True
        )
        if on:
            input = 'What is the weather forecast for ' + result + '? I would like to know all the detail you can get. Make sure they are easily interpretable by people without weather knowledge. Lastly, format the output nicely.'
        else:
            input = 'What is the weather forecast for ' + result + '? Give me a one-sentence summary with nice formatting'
        try:
            result = agent_chain.run(input)
            st.write(result)
        except Exception as e:
            st.write(f"Encountered error while fetching weather data. Please try again later. Error code: {e}")
        
