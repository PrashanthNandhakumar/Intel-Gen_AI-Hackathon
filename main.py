from flask import Flask, render_template, request, jsonify
import os
import requests
from langchain_community.document_loaders import TextLoader
from langchain_huggingface import HuggingFaceEmbeddings
from langchain_chroma import Chroma
from langchain.text_splitter import CharacterTextSplitter
app = Flask(__name__)
PERSIST = True
vectorstore = None
GROQ_API_KEY = "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"

artifact_prompts = {
    "artifact001": "Describe the history and significance of the ancient Greek vase with ID artifact001",
    "artifact002": "What you can do?",
    # Add more artifact IDs and their corresponding prompts here
}


def initialize_vectorstore():
    global vectorstore
    if PERSIST and os.path.exists("persist"):
        print("Reusing index...\n")
        vectorstore = Chroma(persist_directory="persist", embedding_function=HuggingFaceEmbeddings())
    else:
        loader = TextLoader("data.txt")
        documents = loader.load()
        text_splitter = CharacterTextSplitter(chunk_size=1000, chunk_overlap=0)
        texts = text_splitter.split_documents(documents)
        vectorstore = Chroma.from_documents(texts, HuggingFaceEmbeddings(), persist_directory="persist")
def query_groq(prompt):
    url = "https://api.groq.com/openai/v1/chat/completions"
    headers = {
        "Authorization": f"Bearer {GROQ_API_KEY}",
        "Content-Type": "application/json"
    }
    data = {
        "model": "llama-3.1-70b-versatile",
        "messages": [
            {"role": "system", "content": "You are an enthusiastic and friendly assistant.Your Name is Chit Chat Assist. You are going to give all the details about the Artifacts of the Musuems in India.You are the main chatbot for the users to acquire data very easily. Your responses should be energetic, positive, and very natural-sounding."},
            {"role": "user", "content": prompt}
        ],
        "temperature": 0.7
        # Provide humanized answers based on the given context. Do not mention any price details, instead suggest that price information will be shared by respective dealers.
    }
    response = requests.post(url, json=data, headers=headers)
    response.raise_for_status()
    response_json = response.json()
    if 'choices' not in response_json:
        print("Unexpected response format:", response_json)
        return "Sorry, I couldn't process your request at this time."
    
    return response_json['choices'][0]['message']['content']

@app.route('/')
def home():
    return render_template('index.html')

@app.route('/ask', methods=['POST'])
def ask():
    message = request.json['message']
    chat_history = request.json.get('chat_history', [])
    relevant_docs = vectorstore.similarity_search(message, k=2)  # Assuming vectorstore is defined
    context = "\n".join([doc.page_content for doc in relevant_docs])
    prompt = f"""Context: {context}
    Question: {message}
    Answer:"""
    answer = query_groq(prompt)
    chat_history.append((message, answer))
    return jsonify({'answer': answer, 'chat_history': chat_history})

@app.route('/artifact_recognized', methods=['POST'])
def artifact_recognized():
    artifact_data = request.json
    artifact_id = artifact_data.get('artifact_id')
    
    if artifact_id not in artifact_prompts:
        return jsonify({"status": "error", "message": "Unknown artifact ID"}), 400

    prompt = artifact_prompts[artifact_id]
    print(prompt)

     
    # Simulate a request to the /ask endpoint
    with app.test_request_context('/ask', method='POST', json={'message': prompt}):
        response = ask()

    response_data = response.get_json() 
    
    return jsonify({
        "status": "success",
        "message": "Artifact recognized",
        "artifact_id": artifact_id,
        "data": response_data 
    })


if __name__ == '__main__':
    initialize_vectorstore()
    app.run(debug=True)