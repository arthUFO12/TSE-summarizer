from bs4 import BeautifulSoup
import subprocess
import re
import requests
import argparse
import sys
import time

def query_loop(directory, indexFilepath):
    proc = subprocess.Popen(['../C/querier/querier', directory, indexFilepath],
                     stdin=subprocess.PIPE,
                     stdout=subprocess.PIPE,
                     stderr=subprocess.PIPE,
                     text=True
                     )
    
    for query in prompt():
        proc.stdin.write(query + '\n')
        proc.stdin.flush()
        for i in range(2): proc.stdout.readline()
        print("Search results: \n")
        for i in range(3):
            parsed = re.search(r'Score\s+(\d+)\s+\|\s+Doc\s+(\d+):\s+(http[^\s]+)', proc.stdout.readline())
            if not parsed: break
            print(f'AI summary of {parsed.group(3)}:')
            print("\n".join(get_summary(directory + parsed.group(2)).split("\n")[1:]) + "\n")
            time.sleep(2)
            

    proc.stdin.close()

def prompt():
    print("Query? ", end='', flush=True)
    for line in sys.stdin:
        yield line.strip()
        print("Query? ", end='', flush=True)

def get_summary(filePath):
    file = open(filePath, "r")
    title, structure = get_structure(file)
    prompt = generate_prompt(title, structure)

    response = requests.post("http://localhost:11434/api/generate",
                             json={"model": "gemma3:4b", "prompt": prompt, "stream": False},
                             timeout=60
                             )
    
    response.raise_for_status()
    result = response.json()
    summary = result["response"].strip()
    return summary

def generate_prompt(title, structure) -> str:
    return f"""
    You are to generate a summary about a webpage with title {title}.
    The webpage content will be given to you in its original structure and order of elements.
    
    Example content below
    H1: My Favorite Things (Main heading)
    P: My favorite things to do are go to the mall, eat, and play. (Paragraph of Text)
    A: West Farms Mall (Link to a relevant page, in this case a mall I like to go to)
    H2: Eating (Subheaading)
    P: My favorite restaurant is Chili's. (Another paragraph of text)

    The content of the {title} website is \n {structure}

    Return a 5-sentence summary of the website, recounting the most important details. Use the headings and
    paragraphs to inform your decisions of most important details while using link text for extra context.
    Leave out any text not related to this summary in your response.
    """

def get_structure(file) -> tuple[str,str]:

    html = BeautifulSoup(file, 'html.parser')

    title = f'TITLE: {html.find('title').text.strip()}'
    structure = ''
    for tag in html.find('body').descendants:
        if tag.name in {"h1", "h2", "h3", "h4", "p"} and tag.text.strip():
            structure += f'{tag.name.upper()}:  {tag.text.strip()}\n'

    return title, structure


def parseArgs():
    parser = argparse.ArgumentParser()
    parser.add_argument("directory", help="Directory containing html")
    parser.add_argument("indexFilepath", help="Filepath to index")
    return parser.parse_args()

def main():
    args = parseArgs()
    query_loop(args.directory, args.indexFilepath)
    

if __name__ == '__main__':
    main()