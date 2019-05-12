import React, { Component } from 'react'
import { _parseJSON, isEmpty } from "../../HttpAssist"
import { Pager } from "react-bootstrap"
import LikedEntity from '../LikedEntity';

export default class History extends Component {
    constructor(props) {
        super(props);
        this.handleClick = props.handleClick;

        this.state = {
            page: 1
        };
        this.page = 1;
        this.url = "https://kapust1n.ru/api/history?page=";
        this.nextPage = this.nextPage.bind(this);
        this.prevPage = this.prevPage.bind(this);
    }
    componentDidMount() {
        const finalUrl = this.url + this.page;
        console.log("will mount");

        const request = async () => {
            const res = await fetch(finalUrl);
            if (res.status === 200) {
                let comments = await _parseJSON(res);
                this.setState({ comments: comments });
            } else {
                const response = await _parseJSON(res);
                if (response["error"] !== undefined)
                    alert(response["error"]);
                else {
                    alert("smth wrong!");
                }
            }
        }
        request();
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
    render() {
        if (this.state.comments === undefined || isEmpty(this.state.comments)) {
            return (
                'loading'
            )
        }
        else {
            const comments = this.state.comments["history"];
            const commentsElements = comments.map((comment, index) =>
                <li key={comment.timestamp} className="article-list__li">
                    <LikedEntity user={comment.name} timestamp={comment.timestamp} comment={comment.comment} id={index} />
                </li>
            )
            let disabledNext = commentsElements.length !== 10
            let disabledPrev = this.page === 1;

            let pager = (<Pager>
                <Pager.Item disabled={disabledPrev} previous onClick={this.prevPage}>
                    &larr; Previous
            </Pager.Item>
                <Pager.Item disabled={disabledNext} next onClick={this.nextPage}>
                    Next &rarr;
            </Pager.Item>
            </Pager>)
            return (
                <div>
                    {pager}
                    < ul >
                        {commentsElements}
                    </ul >
                </div>
            )
        }

    }
}

