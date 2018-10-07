

## GET

> oneNews

Params: newsId

Desc: Get one news with comments(every comment contain username)

Return codes: 200,403,500

> titles

Params: count

Desc: Get last count titles of news with comment count

Return codes: 200,403

> login

Params: username, password

Desc: Return userId if success

Return codes: 200,403,500


## POST
> like

Params: commentId, userid of author comment

Desc: like comment

Return codes: 200,403,500

> register

Params: username, password

Desc: Return userId if success

Return codes: 200,403,500

> createNews

Params: title, body

Desc: Return newsId if success

Return codes: 200,403,500

> comment

Params: userid, newsid, text

Desc: Comment news

Return codes: 200,403,500


## DELETE
> delete

Params: userId

Desc: Delete user

Return codes: 200,403,500



