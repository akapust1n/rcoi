import React, { Component } from "react";

export default class LikedEntity extends Component {
    constructor(props) {
        super(props);
        this.user = this.props.user;
        this.comment = this.props.comment;
        this.timestamp = this.props.timestamp;
    }


    render() {
        const date = new Date(this.timestamp * 1000);

        return (
            <div>
                <div className="card mx-auto">
                    <div className="card-header">
                        <h4> {this.user} liked comment at {date.toLocaleString()}</h4>
                    </div>
                    <div className="card-body">
                        {this.comment}

                    </div>
                </div>

            </div>
        );
    }
}