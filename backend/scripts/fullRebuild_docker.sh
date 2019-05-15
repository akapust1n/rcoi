docker build . -f Dockerfile -t akapust1n/courcework --no-cache 
 docker build . -f Dockerfile_comments -t akapust1n/courcework-comments --no-cache &&  docker build . -f Dockerfile_news -t akapust1n/courcework-news --no-cache
docker build . -f Dockerfile_likehistory -t akapust1n/courcework-likehistory --no-cache && docker build . -f Dockerfile_users -t akapust1n/courcework-users --no-cache && docker build . -f Dockerfile_gateway -t akapust1n/courcework-gateway --no-cache
