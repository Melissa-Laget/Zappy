FROM python:3.11

RUN pip3 install --upgrade pip

COPY . /app

WORKDIR /app

RUN make requirements

ENTRYPOINT ["python", "zappy_ai"]