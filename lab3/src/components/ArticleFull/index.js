import React, { Component } from 'react'
import { _parseJSON, isEmpty } from "../../HttpAssist"
import Comment from "../Comments"
import CommentsForm from "../CommentForm"
import { Pager } from "react-bootstrap"
import * as Showdown from "showdown";
import Parser from 'html-react-parser';

import './style.css'


export default class ArticleFull extends Component {
    constructor(props) {
        super(props)

        this.state = {
            isLoaded: false,
        }
        this.page = 1;
        this.noComments = false;
        this.url = `https://kapust1n.ru:8079/oneNews?newsId=${props.id}&page=`;
        this.postComment = this.postComment.bind(this);
        this.nextPage = this.nextPage.bind(this);
        this.prevPage = this.prevPage.bind(this);
        this.converter = new Showdown.Converter({
            tables: true,
            simplifiedAutoLink: true,
            strikethrough: true,
            tasklists: true
        });
    }

    componentDidMount() {
        console.log("START mount");
        const finalUrl = this.url + this.page;
        const request = async () => {
            console.log("START REQUEST", finalUrl);
            const res = await fetch(finalUrl);
            console.log("END REQUEST", finalUrl);

            if (res.status === 200) {
                let article = await _parseJSON(res);
                this.article = article;
                this.setState({ isLoaded: true });
            } else {
                const response = await _parseJSON(res);
                alert(response["error"]);
            }
        }
        request();
    }
    postComment() {
        this.setState({ isLoaded: false }, () => {
            this.componentDidMount();
        });
    }
    nextPage() {
        this.page += 1;
        this.setState({ isLoaded: false }, () => {
            this.componentDidMount();
        });
    }
    prevPage() {
        this.page = this.page > 1 ? this.page - 1 : this.page;
        this.setState({ isLoaded: false }, () => {
            this.componentDidMount();
        });
    }
    // componentWillUpdate() {
    //     console.log("START will update");
    //     const finalUrl = this.url + this.page;
    //     const request = async () => {

    //         console.log("START REQUEST", finalUrl);
    //         const res = await fetch(finalUrl);
    //         console.log("REQUEST IS READY");
    //         if (res.status === 200) {
    //             console.log("will upd");
    //             let article = await _parseJSON(res);
    //             this.article = article;
    //             this.setState({ isLoaded: true });

    //             console.log("arcticle before", article);
    //         } else {
    //             alert("cant load news!");
    //         }
    //     }
    //     request();
    // }
    render() {
        if (!this.state.isLoaded || isEmpty(this.article)) {
            return (
                'loading'
            )
        } else {
            const article = this.article.news;
            const comments = this.article.comments;
            const date = new Date(article.timestamp * 1000);
            const commentsHtml = comments.map((comment, index) =>
                <li key={comment.commentId} className="article-list__li">
                    <Comment comment={comment} />
                </li>
            )
            let commentsHeader;
            let pager;
            if (comments.length > 0) {
                commentsHeader = <h3>Comments</h3>
                let disabledNext = comments.length !== 10
                let disabledPrev = this.page === 1;

                pager = (<Pager>
                    <Pager.Item disabled={disabledPrev} previous onClick={this.prevPage}>
                        &larr; Previous
                    </Pager.Item>
                    <Pager.Item disabled={disabledNext} next onClick={this.nextPage}>
                        Next &rarr;
                    </Pager.Item>
                </Pager>)
            }
            const bodyHtml = this.converter.makeHtml(article.body);

            return (
                <div>
                    <div className="card mx-auto">
                        <div className="card-header">
                            <h2> {article.title}</h2>
                        </div>
                        <div className="card-body">
                            {Parser(bodyHtml)}
                            <h6 className="card-subtitle text-muted">
                                creation date: {date.toLocaleString()}
                            </h6>
                        </div>
                    </div>
                    {commentsHeader}{pager}
                    <ul>
                        {commentsHtml}
                    </ul>
                    <CommentsForm userId={1} newsId={article.ID} postComment={this.postComment} />
                </div>
            )
        }
    }

}

