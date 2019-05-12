import React, { Component } from 'react'
import { _parseJSON } from "../../HttpAssist"
import { Panel, Badge, Glyphicon } from "react-bootstrap"

export default class Comment extends Component {
    constructor(props) {
        super(props);

        this.state = {
            login: this.props.comment.name,
            body: this.props.comment.body,
            rating: this.props.comment.rating,
            commentId: this.props.comment.commentId,
            isLiked: false,
        };
        console.log(this.props.comment);
        this.incRating = this.incRating.bind(this);
        this.url = "https://kapust1n.ru/api/like";

    }

    incRating() {
        const commentId = this.state.commentId;
        const data = JSON.stringify({
            commentId: commentId,
        });
        const authToken =
            localStorage.getItem("authtoken");

        fetch(this.url, {
            method: 'POST',
            mode: 'cors',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': 'Bearer ' + authToken
            },
            body: data,
        }).then(async res => {
            if (res.status === 200) {
                this.setState({ rating: this.state.rating + 1, isLiked: true });
                return _parseJSON(res);
            } else {
                this.setState({ isLiked: true });
                if (res.status !== 208) {
                    const response = await _parseJSON(res);
                    alert(response["error"]);
                    throw Error();
                }
            }
        }).catch((error) => {
            console.log("Cant like comment!");
        })
    }
    render() {
        let isLiked
        if (!this.state.isLiked)
            isLiked = this.incRating;
        return (
            <Panel>
                <Panel.Heading>{this.state.login}</Panel.Heading>
                <Panel.Body>{this.state.body}
                    <Badge pullRight={true}>{this.state.rating}</Badge>
                    <Glyphicon onClick={isLiked} glyph="glyphicon glyphicon-heart" className="pull-right"></Glyphicon>
                </Panel.Body>
            </Panel>

        )
    }

}

