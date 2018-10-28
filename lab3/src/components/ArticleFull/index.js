import React, { Component } from 'react'
import _parseJSON from "../../HttpAssist"
import Comment from "../Comments"
import './style.css'


export default class ArticleFull extends Component {
    constructor(props) {
        super(props)

        this.state = {
            isLoaded: false
        }
        console.log('newsid ', props.id);
        this.url = `http://localhost:8080/oneNews?page=${1}&newsId=${props.id}`;
    }

    componentWillMount() {
        const finalUrl = this.url; //TODO ADD PAGINATION
        const request = async () => {
            const res = await fetch(finalUrl);
            if (res.status === 200) {
                let article = await _parseJSON(res);
                console.log("article", article);
                this.article = article;
                this.setState({ isLoaded: true });
            } else {
                alert("cant load news!");
            }
        }
        request();
    }

    render() {
        if (!this.state.isLoaded) {
            return (
                'loading'
            )
        } else {
            const article = this.article.news;
            const comments = this.article.comments;
            console.log(article);
            const date = new Date(article.timestamp * 1000);
            const commentsHtml = comments.map((comment, index) =>
                <li key={comment.commentId} className="article-list__li">
                    <Comment comment={comment} />
                </li>
            )

            return (
                <div>
                    <div className="card mx-auto">
                        <div className="card-header">
                            <h2> {article.title}</h2>
                        </div>
                        <div className="card-body">
                            {article.body}
                            <h6 className="card-subtitle text-muted">
                                creation date: {date.toLocaleString()}
                            </h6>
                        </div>
                    </div>
                    <h3>Comments</h3>
                    <ul>
                        {commentsHtml}
                    </ul>
                </div>
            )
        }
    }

}

