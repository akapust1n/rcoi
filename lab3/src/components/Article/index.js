import React, { Component } from 'react'
import { SecondRoutes } from "../../App"

export default class Article extends Component {
    constructor(props) {
        super(props)

        this.state = {
            count: 0
        }
        this.handleClick = props.handleClick;
        this.articleId = props.id;
    }

    render() {
        const { article } = this.props;
        const date = new Date(article.timestamp * 1000);
        return (
            <div className="card mx-auto">
                <div className="card-header">
                    <h2> <a onClick={() => this.handleClick(SecondRoutes.fullArticle, this.articleId)}> {article.title}</a></h2>
                </div>
                <div className="card-body">
                    <h6 className="card-subtitle ">
                        Comments: {article.count}
                    </h6>
                    <h6 className="card-subtitle text-muted">
                        creation date: {date.toLocaleString()}
                    </h6>
                </div>
            </div >
        )
    }

}

